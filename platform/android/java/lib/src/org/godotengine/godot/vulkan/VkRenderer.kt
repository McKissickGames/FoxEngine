/*************************************************************************/
/*  VkRenderer.kt                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://Foxengine.org                          */
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

@file:JvmName("VkRenderer")
package org.Foxengine.Fox.vulkan

import android.view.Surface

import org.Foxengine.Fox.Fox
import org.Foxengine.Fox.FoxLib
import org.Foxengine.Fox.plugin.FoxPlugin
import org.Foxengine.Fox.plugin.FoxPluginRegistry

/**
 * Responsible to setting up and driving the Vulkan rendering logic.
 *
 * <h3>Threading</h3>
 * The renderer will be called on a separate thread, so that rendering
 * performance is decoupled from the UI thread. Clients typically need to
 * communicate with the renderer from the UI thread, because that's where
 * input events are received. Clients can communicate using any of the
 * standard Java techniques for cross-thread communication, or they can
 * use the  [VkSurfaceView.queueOnVkThread] convenience method.
 *
 * @see [VkSurfaceView.startRenderer]
 */
internal class VkRenderer {
	private val pluginRegistry: FoxPluginRegistry = FoxPluginRegistry.getPluginRegistry()

	/**
	 * Called when the surface is created and signals the beginning of rendering.
	 */
	fun onVkSurfaceCreated(surface: Surface) {
		FoxLib.newcontext(surface, false)

		for (plugin in pluginRegistry.getAllPlugins()) {
			plugin.onVkSurfaceCreated(surface)
		}
	}

	/**
	 * Called after the surface is created and whenever its size changes.
	 */
	fun onVkSurfaceChanged(surface: Surface, width: Int, height: Int) {
		FoxLib.resize(surface, width, height)

		for (plugin in pluginRegistry.getAllPlugins()) {
			plugin.onVkSurfaceChanged(surface, width, height)
		}
	}

	/**
	 * Called to draw the current frame.
	 */
	fun onVkDrawFrame() {
		FoxLib.step()
		for (plugin in pluginRegistry.getAllPlugins()) {
			plugin.onVkDrawFrame()
		}
	}

	/**
	 * Called when the rendering thread is resumed.
	 */
	fun onVkResume() {
		FoxLib.onRendererResumed()
	}

	/**
	 * Called when the rendering thread is paused.
	 */
	fun onVkPause() {
		FoxLib.onRendererPaused()
	}

	/**
	 * Called when the rendering thread is destroyed and used as signal to tear down the Vulkan logic.
	 */
	fun onVkDestroy() {
	}
}
