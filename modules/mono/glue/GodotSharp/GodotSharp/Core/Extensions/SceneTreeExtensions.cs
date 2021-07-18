using System;
using System.Runtime.CompilerServices;
using Fox.Collections;

namespace Fox
{
    public partial class SceneTree
    {
        public Array<T> GetNodesInGroup<T>(StringName group) where T : class
        {
            return new Array<T>(Fox_icall_SceneTree_get_nodes_in_group_Generic(Object.GetPtr(this), StringName.GetPtr(group), typeof(T)));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static IntPtr Fox_icall_SceneTree_get_nodes_in_group_Generic(IntPtr obj, IntPtr group, Type elemType);
    }
}
