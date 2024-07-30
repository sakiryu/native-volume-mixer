# Electron volume

## Requiremers
- Node 20.9
- Visual Studio 2022
  - MSVC v143 build tools
  - ATL for latest v143 build tools
- Python 3 (optional) 
 
## Build

### With build script

Usage:
```python:
python build_and_run.py <Debug|Release> <x86|x64>
```
The script relies on VsDevCmd.bat and msbuild to build the C++ solution.
For the electron project, it uses npm to install the dependencies and run.

### Manually
- Open the volume_mixer.sln and build it
- Navigate to the ui directory, and run

```js:
npm install
npm start <full_path_to_volume_mixer.exe>
```
