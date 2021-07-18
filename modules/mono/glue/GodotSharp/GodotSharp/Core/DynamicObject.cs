
using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;

namespace Fox
{
    /// <summary>
    /// Represents an <see cref="Fox.Object"/> whose members can be dynamically accessed at runtime through the Variant API.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The <see cref="Fox.DynamicFoxObject"/> class enables access to the Variant
    /// members of a <see cref="Fox.Object"/> instance at runtime.
    /// </para>
    /// <para>
    /// This allows accessing the class members using their original names in the engine as well as the members from the
    /// script attached to the <see cref="Fox.Object"/>, regardless of the scripting language it was written in.
    /// </para>
    /// </remarks>
    /// <example>
    /// This sample shows how to use <see cref="Fox.DynamicFoxObject"/> to dynamically access the engine members of a <see cref="Fox.Object"/>.
    /// <code>
    /// dynamic sprite = GetNode("Sprite2D").DynamicFoxObject;
    /// sprite.add_child(this);
    ///
    /// if ((sprite.hframes * sprite.vframes) &gt; 0)
    ///     sprite.frame = 0;
    /// </code>
    /// </example>
    /// <example>
    /// This sample shows how to use <see cref="Fox.DynamicFoxObject"/> to dynamically access the members of the script attached to a <see cref="Fox.Object"/>.
    /// <code>
    /// dynamic childNode = GetNode("ChildNode").DynamicFoxObject;
    ///
    /// if (childNode.print_allowed)
    /// {
    ///     childNode.message = "Hello from C#";
    ///     childNode.print_message(3);
    /// }
    /// </code>
    /// The <c>ChildNode</c> node has the following GDScript script attached:
    /// <code>
    /// // # ChildNode.gd
    /// // var print_allowed = true
    /// // var message = ""
    /// //
    /// // func print_message(times):
    /// //     for i in times:
    /// //         print(message)
    /// </code>
    /// </example>
    public class DynamicFoxObject : DynamicObject
    {
        /// <summary>
        /// Gets the <see cref="Fox.Object"/> associated with this <see cref="Fox.DynamicFoxObject"/>.
        /// </summary>
        public Object Value { get; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Fox.DynamicFoxObject"/> class.
        /// </summary>
        /// <param name="FoxObject">
        /// The <see cref="Fox.Object"/> that will be associated with this <see cref="Fox.DynamicFoxObject"/>.
        /// </param>
        /// <exception cref="System.ArgumentNullException">
        /// Thrown when the <paramref name="FoxObject"/> parameter is null.
        /// </exception>
        public DynamicFoxObject(Object FoxObject)
        {
            if (FoxObject == null)
                throw new ArgumentNullException(nameof(FoxObject));

            this.Value = FoxObject;
        }

        public override IEnumerable<string> GetDynamicMemberNames()
        {
            return Fox_icall_DynamicFoxObject_SetMemberList(Object.GetPtr(Value));
        }

        public override bool TryBinaryOperation(BinaryOperationBinder binder, object arg, out object result)
        {
            switch (binder.Operation)
            {
                case ExpressionType.Equal:
                case ExpressionType.NotEqual:
                    if (binder.ReturnType == typeof(bool) || binder.ReturnType.IsAssignableFrom(typeof(bool)))
                    {
                        if (arg == null)
                        {
                            bool boolResult = Object.IsInstanceValid(Value);

                            if (binder.Operation == ExpressionType.Equal)
                                boolResult = !boolResult;

                            result = boolResult;
                            return true;
                        }

                        if (arg is Object other)
                        {
                            bool boolResult = (Value == other);

                            if (binder.Operation == ExpressionType.NotEqual)
                                boolResult = !boolResult;

                            result = boolResult;
                            return true;
                        }
                    }

                    break;
                default:
                    // We're not implementing operators <, <=, >, and >= (LessThan, LessThanOrEqual, GreaterThan, GreaterThanOrEqual).
                    // These are used on the actual pointers in variant_op.cpp. It's better to let the user do that explicitly.
                    break;
            }

            return base.TryBinaryOperation(binder, arg, out result);
        }

        public override bool TryConvert(ConvertBinder binder, out object result)
        {
            if (binder.Type == typeof(Object))
            {
                result = Value;
                return true;
            }

            if (typeof(Object).IsAssignableFrom(binder.Type))
            {
                // Throws InvalidCastException when the cast fails
                result = Convert.ChangeType(Value, binder.Type);
                return true;
            }

            return base.TryConvert(binder, out result);
        }

        public override bool TryGetIndex(GetIndexBinder binder, object[] indexes, out object result)
        {
            if (indexes.Length == 1)
            {
                if (indexes[0] is string name)
                {
                    return Fox_icall_DynamicFoxObject_GetMember(Object.GetPtr(Value), name, out result);
                }
            }

            return base.TryGetIndex(binder, indexes, out result);
        }

        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            return Fox_icall_DynamicFoxObject_GetMember(Object.GetPtr(Value), binder.Name, out result);
        }

        public override bool TryInvokeMember(InvokeMemberBinder binder, object[] args, out object result)
        {
            return Fox_icall_DynamicFoxObject_InvokeMember(Object.GetPtr(Value), binder.Name, args, out result);
        }

        public override bool TrySetIndex(SetIndexBinder binder, object[] indexes, object value)
        {
            if (indexes.Length == 1)
            {
                if (indexes[0] is string name)
                {
                    return Fox_icall_DynamicFoxObject_SetMember(Object.GetPtr(Value), name, value);
                }
            }

            return base.TrySetIndex(binder, indexes, value);
        }

        public override bool TrySetMember(SetMemberBinder binder, object value)
        {
            return Fox_icall_DynamicFoxObject_SetMember(Object.GetPtr(Value), binder.Name, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static string[] Fox_icall_DynamicFoxObject_SetMemberList(IntPtr FoxObject);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Fox_icall_DynamicFoxObject_InvokeMember(IntPtr FoxObject, string name, object[] args, out object result);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Fox_icall_DynamicFoxObject_GetMember(IntPtr FoxObject, string name, out object result);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Fox_icall_DynamicFoxObject_SetMember(IntPtr FoxObject, string name, object value);

        #region We don't override these methods

        // Looks like this is not usable from C#
        //public override bool TryCreateInstance(CreateInstanceBinder binder, object[] args, out object result);

        // Object members cannot be deleted
        //public override bool TryDeleteIndex(DeleteIndexBinder binder, object[] indexes);
        //public override bool TryDeleteMember(DeleteMemberBinder binder);

        // Invocation on the object itself, e.g.: obj(param)
        //public override bool TryInvoke(InvokeBinder binder, object[] args, out object result);

        // No unnary operations to handle
        //public override bool TryUnaryOperation(UnaryOperationBinder binder, out object result);

        #endregion
    }
}
