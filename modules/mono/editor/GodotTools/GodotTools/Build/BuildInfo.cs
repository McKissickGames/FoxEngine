using System;
using Fox;
using Fox.Collections;
using FoxTools.Internals;
using Path = System.IO.Path;

namespace FoxTools.Build
{
    [Serializable]
    public sealed class BuildInfo : RefCounted // TODO Remove RefCounted once we have proper serialization
    {
        public string Solution { get; }
        public string[] Targets { get; }
        public string Configuration { get; }
        public bool Restore { get; }
        public Array<string> CustomProperties { get; } = new Array<string>(); // TODO Use List once we have proper serialization

        public string LogsDirPath => Path.Combine(FoxSharpDirs.BuildLogsDirs, $"{Solution.MD5Text()}_{Configuration}");

        public override bool Equals(object obj)
        {
            if (obj is BuildInfo other)
                return other.Solution == Solution && other.Targets == Targets &&
                       other.Configuration == Configuration && other.Restore == Restore &&
                       other.CustomProperties == CustomProperties && other.LogsDirPath == LogsDirPath;

            return false;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                int hash = 17;
                hash = hash * 29 + Solution.GetHashCode();
                hash = hash * 29 + Targets.GetHashCode();
                hash = hash * 29 + Configuration.GetHashCode();
                hash = hash * 29 + Restore.GetHashCode();
                hash = hash * 29 + CustomProperties.GetHashCode();
                hash = hash * 29 + LogsDirPath.GetHashCode();
                return hash;
            }
        }

        private BuildInfo()
        {
        }

        public BuildInfo(string solution, string[] targets, string configuration, bool restore)
        {
            Solution = solution;
            Targets = targets;
            Configuration = configuration;
            Restore = restore;
        }
    }
}
