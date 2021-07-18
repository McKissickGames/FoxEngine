using System;

namespace Fox
{
    [AttributeUsage(AttributeTargets.Method)]
    internal class FoxMethodAttribute : Attribute
    {
        private string methodName;

        public string MethodName { get { return methodName; } }

        public FoxMethodAttribute(string methodName)
        {
            this.methodName = methodName;
        }
    }
}
