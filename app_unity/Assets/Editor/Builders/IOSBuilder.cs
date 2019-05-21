using System.Diagnostics;
using System.IO;
using UnityEditor;
using UnityEditor.Callbacks;
using UnityEditor.iOS.Xcode;

class IOSBuilder
{
    //configuration begin
    const string AppPacakgeID = ""; // bundle id
    const string TeamIdentity = ""; // team id
    const string SignIdentity = ""; // command "security find-identity" to find
    const string Provisioning = ""; // name of provision profile exported in xcode
    const string ExportMethod = ""; // "ad-hoc", "app-store", "development" or "enterprise"

    const string ProductsName = "unity";

    static string[] Scenes
    {
        get
        {
            return new string[] { "Assets/Scenes/LaunchScene.unity" };
        }
    }
    //configuration end

    static void Build()
    {
        if (AppPacakgeID.Length == 0
         || TeamIdentity.Length == 0
         || SignIdentity.Length == 0
         || Provisioning.Length == 0
         || ExportMethod.Length == 0)
        {
            return;
        }

        //1st step
        GenerateXcodeProject();
    }

    [PostProcessBuild]
    static void ExportIPA(BuildTarget buildTarget, string projectDirectory)
    {
        if (buildTarget != BuildTarget.iOS) return;

        //2nd step
        EditProjectSettings(projectDirectory);

        //3rd step
        WriteExportOptions(projectDirectory);

        //4st step
        ExportIPA(projectDirectory);

        //5st step
        MoveProducts(projectDirectory);
    }

    static void GenerateXcodeProject()
    {
        string directory = "XCODEBUILD";
        if (Directory.Exists(directory))
        {
            Directory.Delete(directory, true);
        }

        PlayerSettings.applicationIdentifier = AppPacakgeID;
        BuildPipeline.BuildPlayer(Scenes, directory, BuildTarget.iOS, BuildOptions.None);
    }

    static void EditProjectSettings(string directory)
    {
        var file = directory + "/Unity-iPhone.xcodeproj/project.pbxproj";
        var settings = new PBXProject();
        settings.ReadFromFile(file);

        string target = settings.TargetGuidByName(PBXProject.GetUnityTargetName());

        settings.SetBuildProperty(target, "CODE_SIGN_STYLE"               , "Manual"    );
        settings.SetBuildProperty(target, "DEVELOPMENT_TEAM"              , TeamIdentity);
        settings.SetBuildProperty(target, "CODE_SIGN_IDENTITY"            , SignIdentity);
        settings.SetBuildProperty(target, "PROVISIONING_PROFILE_SPECIFIER", Provisioning);

        settings.WriteToFile(file);
    }

    static void WriteExportOptions(string directory)
    {
        string options = "";
        options +=/**/"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        options += "\n <!DOCTYPE plist PUBLIC";
        options += "\n     \"-//Apple//DTD PLIST 1.0//EN\"";
        options += "\n     \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">";
        options += "\n <plist version=\"1.0\">";
        options += "\n <dict>";
        options += "\n     <key>teamID</key> <string>$(teamID)</string>";
        options += "\n     <key>method</key> <string>$(method)</string>";
        options += "\n     <key>provisioningProfiles</key>";
        options += "\n     <dict>";
        options += "\n         <key>$(bundleID)</key> <string>$(provision)</string>";
        options += "\n     </dict>";
        options += "\n     <key>compileBitcode</key> <true />";
        options += "\n     <key>uploadSymbols</key> <true />";
        options += "\n </dict>";
        options += "\n </plist>";

        options = options.Replace("$(teamID)"   , TeamIdentity);
        options = options.Replace("$(method)"   , ExportMethod);
        options = options.Replace("$(bundleID)" , AppPacakgeID);
        options = options.Replace("$(provision)", Provisioning);

        File.WriteAllText(directory + "/ExportOptions.plist", options);
    }

    static void ExportIPA(string directory)
    {
        string scriptPath = directory + "/Export.sh";

        string script = "";
        script +=/**/"#!/bin/bash";
        script += "\n ";
        script += "\n set -e -u";
        script += "\n ";
        script += "\n cd `dirname $0`";
        script += "\n ";
        script += "\n cmd_line=\"xcodebuild\"";
        script += "\n cmd_line=\"$cmd_line -project Unity-iPhone.xcodeproj\"";
        script += "\n cmd_line=\"$cmd_line -scheme Unity-iPhone\"";
        script += "\n cmd_line=\"$cmd_line clean\"";
        script += "\n cmd_line=\"$cmd_line archive\"";
        script += "\n cmd_line=\"$cmd_line -archivePath Unity-iPhone.xcarchive\"";
        script += "\n $cmd_line";
        script += "\n ";
        script += "\n cmd_line=\"xcodebuild\"";
        script += "\n cmd_line=\"$cmd_line -exportArchive\"";
        script += "\n cmd_line=\"$cmd_line -archivePath Unity-iPhone.xcarchive\"";
        script += "\n cmd_line=\"$cmd_line -exportPath Export\"";
        script += "\n cmd_line=\"$cmd_line -exportOptionsPlist ExportOptions.plist\"";
        script += "\n $cmd_line";

        File.WriteAllText(scriptPath, script);

        Process process = new Process();
        process.StartInfo.UseShellExecute = true;
        process.StartInfo.FileName = "sh";
        process.StartInfo.Arguments = scriptPath;
        process.Start();
        process.WaitForExit();
        process.Close();
    }

    static void MoveProducts(string fromDir)
    {
        string toDir = "../BUILD";
        string toSymbol = toDir + "/" + ProductsName + ".dSYM";
        string toAppIPA = toDir + "/" + ProductsName + ".ipa";

        if (!Directory.Exists(toDir))
        {
            Directory.CreateDirectory(toDir);
        }
        if (Directory.Exists(toSymbol))
        {
            Directory.Delete(toSymbol, true);
        }
        if (File.Exists(toAppIPA))
        {
            File.Delete(toAppIPA);
        }

        int nameStart = AppPacakgeID.LastIndexOf('.') + 1;
        string name = AppPacakgeID.Substring(nameStart);

        string fromSymbol = fromDir + "/Unity-iPhone.xcarchive/dSYMs/" + name + ".app.dSYM";
        string fromAppIPA = fromDir + "/Export/Unity-iPhone.ipa";

        Directory.Move(fromSymbol, toSymbol);
        File.Move(fromAppIPA, toAppIPA);
    }
}
