/*************************************************************************/
/*  FoxPluginRegistry.java                                             */
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

import org.Foxengine.Fox.Fox;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.Nullable;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Registry used to load and access the registered Fox Android plugins.
 */
public final class FoxPluginRegistry {
	private static final String TAG = FoxPluginRegistry.class.getSimpleName();

	private static final String Fox_PLUGIN_V1_NAME_PREFIX = "org.Foxengine.plugin.v1.";

	private static FoxPluginRegistry instance;
	private final ConcurrentHashMap<String, FoxPlugin> registry;

	private FoxPluginRegistry(Fox Fox) {
		registry = new ConcurrentHashMap<>();
		loadPlugins(Fox);
	}

	/**
	 * Retrieve the plugin tied to the given plugin name.
	 * @param pluginName Name of the plugin
	 * @return {@link FoxPlugin} handle if it exists, null otherwise.
	 */
	@Nullable
	public FoxPlugin getPlugin(String pluginName) {
		return registry.get(pluginName);
	}

	/**
	 * Retrieve the full set of loaded plugins.
	 */
	public Collection<FoxPlugin> getAllPlugins() {
		return registry.values();
	}

	/**
	 * Parse the manifest file and load all included Fox Android plugins.
	 * <p>
	 * A plugin manifest entry is a '<meta-data>' tag setup as described in the {@link FoxPlugin}
	 * documentation.
	 *
	 * @param Fox Fox instance
	 * @return A singleton instance of {@link FoxPluginRegistry}. This ensures that only one instance
	 * of each Fox Android plugins is available at runtime.
	 */
	public static FoxPluginRegistry initializePluginRegistry(Fox Fox) {
		if (instance == null) {
			instance = new FoxPluginRegistry(Fox);
		}

		return instance;
	}

	/**
	 * Return the plugin registry if it's initialized.
	 * Throws a {@link IllegalStateException} exception if not.
	 *
	 * @throws IllegalStateException if {@link FoxPluginRegistry#initializePluginRegistry(Fox)} has not been called prior to calling this method.
	 */
	public static FoxPluginRegistry getPluginRegistry() throws IllegalStateException {
		if (instance == null) {
			throw new IllegalStateException("Plugin registry hasn't been initialized.");
		}

		return instance;
	}

	private void loadPlugins(Fox Fox) {
		try {
			final Activity activity = Fox.getActivity();
			ApplicationInfo appInfo = activity
											  .getPackageManager()
											  .getApplicationInfo(activity.getPackageName(),
													  PackageManager.GET_META_DATA);
			Bundle metaData = appInfo.metaData;
			if (metaData == null || metaData.isEmpty()) {
				return;
			}

			int FoxPluginV1NamePrefixLength = Fox_PLUGIN_V1_NAME_PREFIX.length();
			for (String metaDataName : metaData.keySet()) {
				// Parse the meta-data looking for entry with the Fox plugin name prefix.
				if (metaDataName.startsWith(Fox_PLUGIN_V1_NAME_PREFIX)) {
					String pluginName = metaDataName.substring(FoxPluginV1NamePrefixLength).trim();
					Log.i(TAG, "Initializing Fox plugin " + pluginName);

					// Retrieve the plugin class full name.
					String pluginHandleClassFullName = metaData.getString(metaDataName);
					if (!TextUtils.isEmpty(pluginHandleClassFullName)) {
						try {
							// Attempt to create the plugin init class via reflection.
							@SuppressWarnings("unchecked")
							Class<FoxPlugin> pluginClass = (Class<FoxPlugin>)Class
																	 .forName(pluginHandleClassFullName);
							Constructor<FoxPlugin> pluginConstructor = pluginClass
																				 .getConstructor(Fox.class);
							FoxPlugin pluginHandle = pluginConstructor.newInstance(Fox);

							// Load the plugin initializer into the registry using the plugin name as key.
							if (!pluginName.equals(pluginHandle.getPluginName())) {
								Log.w(TAG,
										"Meta-data plugin name does not match the value returned by the plugin handle: " + pluginName + " =/= " + pluginHandle.getPluginName());
							}
							registry.put(pluginName, pluginHandle);
							Log.i(TAG, "Completed initialization for Fox plugin " + pluginHandle.getPluginName());
						} catch (ClassNotFoundException e) {
							Log.w(TAG, "Unable to load Fox plugin " + pluginName, e);
						} catch (IllegalAccessException e) {
							Log.w(TAG, "Unable to load Fox plugin " + pluginName, e);
						} catch (InstantiationException e) {
							Log.w(TAG, "Unable to load Fox plugin " + pluginName, e);
						} catch (NoSuchMethodException e) {
							Log.w(TAG, "Unable to load Fox plugin " + pluginName, e);
						} catch (InvocationTargetException e) {
							Log.w(TAG, "Unable to load Fox plugin " + pluginName, e);
						}
					} else {
						Log.w(TAG, "Invalid plugin loader class for " + pluginName);
					}
				}
			}
		} catch (PackageManager.NameNotFoundException e) {
			Log.e(TAG, "Unable load Fox Android plugins from the manifest file.", e);
		}
	}
}
