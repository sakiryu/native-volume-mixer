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

def build_cpp_project(solution_path, configuration, platform):
    vs_dev_cmd = find_vs_dev_cmd()

    if not vs_dev_cmd:
        print("Error: Could not find VsDevCmd.bat")
        sys.exit(1)

    build_command = f'call "{vs_dev_cmd}" && msbuild "{solution_path}" /p:Configuration={configuration} /p:Platform={platform}'

    try:
        result = subprocess.run(build_command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print("Error during build:", e)
        sys.exit(e.returncode)

    project_dir = os.path.dirname(os.path.abspath(__file__))
    build_dir = os.path.join(project_dir, 'native', platform if platform  == 'x64' else '', configuration)

    return build_dir

def build_electron_project(executable_path):
    os.chdir('ui')
    try:
        if not os.path.exists('node_modules'):
            subprocess.check_call(['npm', 'install'], shell=True)
        
        subprocess.check_call(['npm', 'start', executable_path], shell=True)
    except subprocess.CalledProcessError as e:
        print("Electron build failed")
        sys.exit(e.returncode)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python build.py <Debug|Release>")
        sys.exit(1)

    configuration = sys.argv[1]
    if configuration not in ("Debug", "Release"):
        print("Error: Configuration must be either 'Debug' or 'Release'.")
        sys.exit(1)

    platform = sys.argv[2]
    if platform not in ("x86", "x64"):
        print("Error: Platform must be either 'x86' or 'x64'.")
        sys.exit(1)

    solution_file = "native/volume_mixer.sln"
    
    if not os.path.exists(solution_file):
        print(f"Error: Solution file {solution_file} does not exist.")
        sys.exit(1)

    build_dir = build_cpp_project(solution_file, configuration, platform)
    executable_path = os.path.join(build_dir, 'volume_mixer.exe')

    build_electron_project(executable_path)
