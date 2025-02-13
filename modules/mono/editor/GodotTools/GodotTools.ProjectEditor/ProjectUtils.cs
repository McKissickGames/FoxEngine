using System;
using Microsoft.Build.Construction;

namespace FoxTools.ProjectEditor
{
    public sealed class MSBuildProject
    {
        internal ProjectRootElement Root { get; set; }

        public bool HasUnsavedChanges { get; set; }

        public void Save() => Root.Save();

        public MSBuildProject(ProjectRootElement root)
        {
            Root = root;
        }
    }

    public static class ProjectUtils
    {
        public static MSBuildProject Open(string path)
        {
            var root = ProjectRootElement.Open(path);
            return root != null ? new MSBuildProject(root) : null;
        }

        public static void MigrateToProjectSdksStyle(MSBuildProject project, string projectName)
        {
            var origRoot = project.Root;

            if (!string.IsNullOrEmpty(origRoot.Sdk))
                return;

            project.Root = ProjectGenerator.GenGameProject(projectName);
            project.Root.FullPath = origRoot.FullPath;
            project.HasUnsavedChanges = true;
        }

        public static void EnsureFoxSdkIsUpToDate(MSBuildProject project)
        {
            var root = project.Root;
            string FoxSdkAttrValue = ProjectGenerator.FoxSdkAttrValue;

            if (!string.IsNullOrEmpty(root.Sdk) && root.Sdk.Trim().Equals(FoxSdkAttrValue, StringComparison.OrdinalIgnoreCase))
                return;

            root.Sdk = FoxSdkAttrValue;
            project.HasUnsavedChanges = true;
        }
    }
}
