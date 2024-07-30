import os
import subprocess
import sys

def find_vs_dev_cmd():
    program_files = os.environ.get('ProgramFiles(x86)') or os.environ.get('ProgramFiles')
    vswhere_path = os.path.join(program_files, "Microsoft Visual Studio", "Installer", "vswhere.exe")

    if not os.path.exists(vswhere_path):
        print("Error: Could not find vswhere.exe")
        sys.exit(1)

    vswhere_cmd = [
        vswhere_path,
        "-latest",
        "-products", "*",
        "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
        "-property", "installationPath"
    ]

    try:
        vs_install_path = subprocess.check_output(vswhere_cmd, universal_newlines=True).strip()
        if vs_install_path:
            vs_dev_cmd = os.path.join(vs_install_path, "Common7", "Tools", "VsDevCmd.bat")
            return vs_dev_cmd
    except subprocess.CalledProcessError as e:
        print("Error: Could not run vswhere.exe:", e)
        sys.exit(1)

    return None