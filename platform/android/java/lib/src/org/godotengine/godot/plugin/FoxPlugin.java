/*************************************************************************/
/*  FoxPlugin.java                                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

package org.Foxengine.Fox.plugin;

import org.Foxengine.Fox.BuildConfig;
import org.Foxengine.Fox.Fox;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Base class for the Fox Android plugins.
 * <p>
 * A Fox Android plugin is a regular Android library packaged as an aar archive file with the following caveats:
 * <p>
 * - The library must have a dependency on the Fox Android library (Fox-lib.aar).
 * A stable version is available for each release.
 * <p>
 * - The library must include a <meta-data> tag in its manifest file setup as follow:
 * <meta-data android:name="org.Foxengine.plugin.v1.[PluginName]" android:value="[plugin.init.ClassFullName]" />
 * Where:
 * - 'PluginName' is the name of the plugin.
 * - 'plugin.init.ClassFullName' is the full name (package + class name) of the plugin class
 * extending {@link FoxPlugin}.
 *
 * A plugin can also define and provide c/c++ gdnative libraries and nativescripts for the target
 * app/game to leverage.
 * The shared library for the gdnative library will be automatically bundled by the aar build
 * system.
 * Fox '*.gdnlib' and '*.gdns' resource files must however be manually defined in the project
 * 'assets' directory. The recommended path for these resources in the 'assets' directory should be:
 * 'Fox/plugin/v1/[PluginName]/'
 */
public abstract class FoxPlugin {
	private static final String TAG = FoxPlugin.class.getSimpleName();

	private final Fox Fox;
	private final ConcurrentHashMap<String, SignalInfo> registeredSignals = new ConcurrentHashMap<>();

	public FoxPlugin(Fox Fox) {
		this.Fox = Fox;
	}

	/**
	 * Provides access to the Fox engine.
	 */
	protected Fox getFox() {
		return Fox;
	}

	/**
	 * Provides access to the underlying {@link Activity}.
	 */
	@Nullable
	protected Activity getActivity() {
		return Fox.getActivity();
	}

	/**
	 * Register the plugin with Fox native code.
	 *
	 * This method is invoked on the render thread.
	 */
	public final void onRegisterPluginWithFoxNative() {
		registeredSignals.putAll(
				registerPluginWithFoxNative(this, getPluginName(), getPluginMethods(), getPluginSignals(),
						getPluginGDNativeLibrariesPaths()));
	}

	/**
	 * Register the plugin with Fox native code.
	 *
	 * This method must be invoked on the render thread.
	 */
	public static void registerPluginWithFoxNative(Object pluginObject,
			FoxPluginInfoProvider pluginInfoProvider) {
		registerPluginWithFoxNative(pluginObject, pluginInfoProvider.getPluginName(),
				Collections.emptyList(), pluginInfoProvider.getPluginSignals(),
				pluginInfoProvider.getPluginGDNativeLibrariesPaths());

		// Notify that registration is complete.
		pluginInfoProvider.onPluginRegistered();
	}

	private static Map<String, SignalInfo> registerPluginWithFoxNative(Object pluginObject,
			String pluginName, List<String> pluginMethods, Set<SignalInfo> pluginSignals,
			Set<String> pluginGDNativeLibrariesPaths) {
		nativeRegisterSingleton(pluginName, pluginObject);

		Set<Method> filteredMethods = new HashSet<>();
		Class<?> clazz = pluginObject.getClass();

		Method[] methods = clazz.getDeclaredMethods();
		for (Method method : methods) {
			// Check if the method is annotated with {@link UsedByFox}.
			if (method.getAnnotation(UsedByFox.class) != null) {
				filteredMethods.add(method);
			} else {
				// For backward compatibility, process the methods from the given <pluginMethods> argument.
				for (String methodName : pluginMethods) {
					if (methodName.equals(method.getName())) {
						filteredMethods.add(method);
						break;
					}
				}
			}
		}

		for (Method method : filteredMethods) {
			List<String> ptr = new ArrayList<>();

			Class<?>[] paramTypes = method.getParameterTypes();
			for (Class<?> c : paramTypes) {
				ptr.add(c.getName());
			}

			String[] pt = new String[ptr.size()];
			ptr.toArray(pt);

			nativeRegisterMethod(pluginName, method.getName(), method.getReturnType().getName(), pt);
		}

		// Register the signals for this plugin.
		Map<String, SignalInfo> registeredSignals = new HashMap<>();
		for (SignalInfo signalInfo : pluginSignals) {
			String signalName = signalInfo.getName();
			nativeRegisterSignal(pluginName, signalName, signalInfo.getParamTypesNames());
			registeredSignals.put(signalName, signalInfo);
		}

		// Get the list of gdnative libraries to register.
		if (!pluginGDNativeLibrariesPaths.isEmpty()) {
			nativeRegisterGDNativeLibraries(pluginGDNativeLibrariesPaths.toArray(new String[0]));
		}

		return registeredSignals;
	}

	/**
	 * Invoked once during the Fox Android initialization process after creation of the
	 * {@link org.Foxengine.Fox.FoxRenderView} view.
	 * <p>
	 * The plugin can return a non-null {@link View} layout in order to add it to the Fox view
	 * hierarchy.
	 *
	 * @see Activity#onCreate(Bundle)
	 * @return the plugin's view to be included; null if no views should be included.
	 */
	@Nullable
	public View onMainCreate(Activity activity) {
		return null;
	}

	/**
	 * @see Activity#onActivityResult(int, int, Intent)
	 */
	public void onMainActivityResult(int requestCode, int resultCode, Intent data) {
	}

	/**
	 * @see Activity#onRequestPermissionsResult(int, String[], int[])
	 */
	public void onMainRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
	}

	/**
	 * @see Activity#onPause()
	 */
	public void onMainPause() {}

	/**
	 * @see Activity#onResume()
	 */
	public void onMainResume() {}

	/**
	 * @see Activity#onDestroy()
	 */
	public void onMainDestroy() {}

	/**
	 * @see Activity#onBackPressed()
	 */
	public boolean onMainBackPressed() { return false; }

	/**
	 * Invoked on the render thread when the Fox setup is complete.
	 */
	public void onFoxSetupCompleted() {}

	/**
	 * Invoked on the render thread when the Fox main loop has started.
	 */
	public void onFoxMainLoopStarted() {}

	/**
	 * Invoked once per frame on the GL thread after the frame is drawn.
	 */
	public void onGLDrawFrame(GL10 gl) {}

	/**
	 * Called on the GL thread after the surface is created and whenever the OpenGL ES surface size
	 * changes.
	 */
	public void onGLSurfaceChanged(GL10 gl, int width, int height) {}

	/**
	 * Called on the GL thread when the surface is created or recreated.
	 */
	public void onGLSurfaceCreated(GL10 gl, EGLConfig config) {}

	/**
	 * Invoked once per frame on the Vulkan thread after the frame is drawn.
	 */
	public void onVkDrawFrame() {}

	/**
	 * Called on the Vulkan thread after the surface is created and whenever the surface size
	 * changes.
	 */
	public void onVkSurfaceChanged(Surface surface, int width, int height) {}

	/**
	 * Called on the Vulkan thread when the surface is created or recreated.
	 */
	public void onVkSurfaceCreated(Surface surface) {}

	/**
	 * Returns the name of the plugin.
	 * <p>
	 * This value must match the one listed in the plugin '<meta-data>' manifest entry.
	 */
	@NonNull
	public abstract String getPluginName();

	/**
	 * Returns the list of methods to be exposed to Fox.
	 *
	 * @deprecated Used the {@link UsedByFox} annotation instead.
	 */
	@NonNull
	@Deprecated
	public List<String> getPluginMethods() {
		return Collections.emptyList();
	}

	/**
	 * Returns the list of signals to be exposed to Fox.
	 */
	@NonNull
	public Set<SignalInfo> getPluginSignals() {
		return Collections.emptySet();
	}

	/**
	 * Returns the paths for the plugin's gdnative libraries.
	 *
	 * The paths must be relative to the 'assets' directory and point to a '*.gdnlib' file.
	 */
	@NonNull
	protected Set<String> getPluginGDNativeLibrariesPaths() {
		return Collections.emptySet();
	}

	/**
	 * Runs the specified action on the UI thread. If the current thread is the UI
	 * thread, then the action is executed immediately. If the current thread is
	 * not the UI thread, the action is posted to the event queue of the UI thread.
	 *
	 * @param action the action to run on the UI thread
	 */
	protected void runOnUiThread(Runnable action) {
		Fox.runOnUiThread(action);
	}

	/**
	 * Queue the specified action to be run on the render thread.
	 *
	 * @param action the action to run on the render thread
	 */
	protected void runOnRenderThread(Runnable action) {
		Fox.runOnRenderThread(action);
	}

	/**
	 * Emit a registered Fox signal.
	 * @param signalName Name of the signal to emit. It will be validated against the set of registered signals.
	 * @param signalArgs Arguments used to populate the emitted signal. The arguments will be validated against the {@link SignalInfo} matching the registered signalName parameter.
	 */
	protected void emitSignal(final String signalName, final Object... signalArgs) {
		try {
			// Check that the given signal is among the registered set.
			SignalInfo signalInfo = registeredSignals.get(signalName);
			if (signalInfo == null) {
				throw new IllegalArgumentException(
						"Signal " + signalName + " is not registered for this plugin.");
			}
			emitSignal(getFox(), getPluginName(), signalInfo, signalArgs);
		} catch (IllegalArgumentException exception) {
			Log.w(TAG, exception.getMessage());
			if (BuildConfig.DEBUG) {
				throw exception;
			}
		}
	}

	/**
	 * Emit a Fox signal.
	 * @param Fox
	 * @param pluginName Name of the Fox plugin the signal will be emitted from. The plugin must already be registered with the Fox engine.
	 * @param signalInfo Information about the signal to emit.
	 * @param signalArgs Arguments used to populate the emitted signal. The arguments will be validated against the given {@link SignalInfo} parameter.
	 */
	public static void emitSignal(Fox Fox, String pluginName, SignalInfo signalInfo, final Object... signalArgs) {
		try {
			if (signalInfo == null) {
				throw new IllegalArgumentException("Signal must be non null.");
			}

			// Validate the arguments count.
			Class<?>[] signalParamTypes = signalInfo.getParamTypes();
			if (signalArgs.length != signalParamTypes.length) {
				throw new IllegalArgumentException(
						"Invalid arguments count. Should be " + signalParamTypes.length + "  but is " + signalArgs.length);
			}

			// Validate the argument's types.
			for (int i = 0; i < signalParamTypes.length; i++) {
				if (!signalParamTypes[i].isInstance(signalArgs[i])) {
					throw new IllegalArgumentException(
							"Invalid type for argument #" + i + ". Should be of type " + signalParamTypes[i].getName());
				}
			}

			Fox.runOnRenderThread(() -> nativeEmitSignal(pluginName, signalInfo.getName(), signalArgs));

		} catch (IllegalArgumentException exception) {
			Log.w(TAG, exception.getMessage());
			if (BuildConfig.DEBUG) {
				throw exception;
			}
		}
	}

	/**
	 * Used to setup a {@link FoxPlugin} instance.
	 * @param p_name Name of the instance.
	 */
	private static native void nativeRegisterSingleton(String p_name, Object object);

	/**
	 * Used to complete registration of the {@link FoxPlugin} instance's methods.
	 * @param p_sname Name of the instance
	 * @param p_name Name of the method to register
	 * @param p_ret Return type of the registered method
	 * @param p_params Method parameters types
	 */
	private static native void nativeRegisterMethod(String p_sname, String p_name, String p_ret, String[] p_params);

	/**
	 * Used to register gdnative libraries bundled by the plugin.
	 * @param gdnlibPaths Paths to the libraries relative to the 'assets' directory.
	 */
	private static native void nativeRegisterGDNativeLibraries(String[] gdnlibPaths);

	/**
	 * Used to complete registration of the {@link FoxPlugin} instance's methods.
	 * @param pluginName Name of the plugin
	 * @param signalName Name of the signal to register
	 * @param signalParamTypes Signal parameters types
	 */
	private static native void nativeRegisterSignal(String pluginName, String signalName, String[] signalParamTypes);

	/**
	 * Used to emit signal by {@link FoxPlugin} instance.
	 * @param pluginName Name of the plugin
	 * @param signalName Name of the signal to emit
	 * @param signalParams Signal parameters
	 */
	private static native void nativeEmitSignal(String pluginName, String signalName, Object[] signalParams);
}
