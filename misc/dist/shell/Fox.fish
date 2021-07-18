# Fish completion for the Fox editor
# To use it, install this file in `~/.config/fish/completions` then restart your shell.
# You can also `source` this file directly in your shell startup file.
#
# Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
# Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

function Fox_video_driver_args
    # Use a function instead of a fixed string to customize the argument descriptions.
    echo -e "Vulkan\tVulkan renderer"
    echo -e "GLES2\tOpenGL ES 2.0 renderer"
end

# Erase existing completions for Fox.
complete -c Fox -e

# General options:
complete -c Fox -s h -l help -d "Display the full help message"
complete -c Fox -l version -d "Display the version string"
complete -c Fox -s v -l verbose -d "Use verbose stdout mode"
complete -c Fox -l quiet -d "Quiet mode, silences stdout messages (errors are still displayed)"

# Run options:
complete -c Fox -s e -l editor -d "Start the editor instead of running the scene"
complete -c Fox -s p -l project-manager -d "Start the project manager, even if a project is auto-detected"
complete -c Fox -s q -l quit -d "Quit after the first iteration"
complete -c Fox -s l -l language -d "Use a specific locale (<locale> being a two-letter code)" -x
complete -c Fox -l path -d "Path to a project (<directory> must contain a 'project.Fox' file)" -r
complete -c Fox -s u -l upwards -d "Scan folders upwards for project.Fox file"
complete -c Fox -l main-pack -d "Path to a pack (.pck) file to load" -r
complete -c Fox -l render-thread -d "Set the render thread mode" -x -a "unsafe safe separate"
complete -c Fox -l remote-fs -d "Use a remote filesystem (<host/IP>[:<port>] address)" -x
complete -c Fox -l remote-fs-password -d "Password for remote filesystem" -x
complete -c Fox -l audio-driver -d "Set the audio driver" -x
complete -c Fox -l video-driver -d "Set the video driver" -x -a "(Fox_video_driver_args)"

# Display options:
complete -c Fox -s f -l fullscreen -d "Request fullscreen mode"
complete -c Fox -s m -l maximized -d "Request a maximized window"
complete -c Fox -s w -l windowed -d "Request windowed mode"
complete -c Fox -s t -l always-on-top -d "Request an always-on-top window"
complete -c Fox -l resolution -d "Request window resolution" -x
complete -c Fox -l position -d "Request window position" -x
complete -c Fox -l low-dpi -d "Force low-DPI mode (macOS and Windows only)"
complete -c Fox -l no-window -d "Disable window creation (Windows only), useful together with --script"

# Debug options:
complete -c Fox -s d -l debug -d "Debug (local stdout debugger)"
complete -c Fox -s b -l breakpoints -d "Specify the breakpoint list as source::line comma-separated pairs, no spaces (use %20 instead)" -x
complete -c Fox -l profiling -d "Enable profiling in the script debugger"
complete -c Fox -l remote-debug -d "Enable remote debugging"
complete -c Fox -l debug-collisions -d "Show collision shapes when running the scene"
complete -c Fox -l debug-navigation -d "Show navigation polygons when running the scene"
complete -c Fox -l frame-delay -d "Simulate high CPU load (delay each frame by the given number of milliseconds)" -x
complete -c Fox -l time-scale -d "Force time scale (higher values are faster, 1.0 is normal speed)" -x
complete -c Fox -l disable-render-loop -d "Disable render loop so rendering only occurs when called explicitly from script"
complete -c Fox -l disable-crash-handler -d "Disable crash handler when supported by the platform code"
complete -c Fox -l fixed-fps -d "Force a fixed number of frames per second (this setting disables real-time synchronization)" -x
complete -c Fox -l print-fps -d "Print the frames per second to the stdout"

# Standalone tools:
complete -c Fox -s s -l script -d "Run a script" -r
complete -c Fox -l check-only -d "Only parse for errors and quit (use with --script)"
complete -c Fox -l export -d "Export the project using the given preset and matching release template" -x
complete -c Fox -l export-debug -d "Same as --export, but using the debug template" -x
complete -c Fox -l export-pack -d "Same as --export, but only export the game pack for the given preset" -x
complete -c Fox -l doctool -d "Dump the engine API reference to the given path in XML format, merging if existing files are found" -r
complete -c Fox -l no-docbase -d "Disallow dumping the base types (used with --doctool)"
complete -c Fox -l build-solutions -d "Build the scripting solutions (e.g. for C# projects)"
complete -c Fox -l gdnative-generate-json-api -d "Generate JSON dump of the Fox API for GDNative bindings"
complete -c Fox -l test -d "Run a unit test" -x
