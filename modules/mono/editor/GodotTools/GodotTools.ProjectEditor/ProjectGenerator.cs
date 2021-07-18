using System;
using System.IO;
using System.Text;
using Microsoft.Build.Construction;
using Microsoft.Build.Evaluation;
using FoxTools.Shared;

namespace FoxTools.ProjectEditor
{
    public static class ProjectGenerator
    {
        public static string FoxSdkAttrValue => $"Fox.NET.Sdk/{GeneratedFoxNupkgsVersions.FoxNETSdk}";

        public static ProjectRootElement GenGameProject(string name)
        {
            if (name.Length == 0)
                throw new ArgumentException("Project name is empty", nameof(name));

            var root = ProjectRootElement.Create(NewProjectFileOptions.None);

            root.Sdk = FoxSdkAttrValue;

            var mainGroup = root.AddPropertyGroup();
            mainGroup.AddProperty("TargetFramework", "netstandard2.1");

            string sanitizedName = IdentifierUtils.SanitizeQualifiedIdentifier(name, allowEmptyIdentifiers: true);

            // If the name is not a valid namespace, manually set RootNamespace to a sanitized one.
            if (sanitizedName != name)
                mainGroup.AddProperty("RootNamespace", sanitizedName);

            return root;
        }

        public static string GenAndSaveGameProject(string dir, string name)
        {
            if (name.Length == 0)
                throw new ArgumentException("Project name is empty", nameof(name));

            string path = Path.Combine(dir, name + ".csproj");

            var root = GenGameProject(name);

            // Save (without BOM)
            root.Save(path, new UTF8Encoding(encoderShouldEmitUTF8Identifier: false));

            return Guid.NewGuid().ToString().ToUpper();
        }
    }
}
