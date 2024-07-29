const { app, BrowserWindow, ipcMain  } = require('electron');
const VolumeMixerService = require('./volume-mixer-service');
const { spawn } = require('child_process');
const path = require('path');

const spawnVolumeMixerProcess = () => {
    const volumeMixerPath = path.join(__dirname, '..', 'native', 'x64', 'Debug', 'volume_mixer.exe');

    const volumeMixerProcess = spawn(volumeMixerPath);
    volumeMixerProcess.on('close', (code) => {
        console.log(`Child process exited with code ${code}`);
    });

    volumeMixerProcess.on('error', (err) => {
        console.error('Failed to start child process:', err);
    });
}

const createWindow = () => {
    const win = new BrowserWindow({
      width: 230,
      height: 170,
      webPreferences: {
        preload: path.join(__dirname, 'preload.js')
      }    
    })
    win.loadFile('index.html')
    win.removeMenu();
}

app.whenReady().then(async () => {
    spawnVolumeMixerProcess();

    const service = new VolumeMixerService();

    app.on('window-all-closed', async () => {
        await service.closeApp()
        app.quit();
    });

    ipcMain.handle('set-volume', async (_, volume) => await service.setVolume(volume));
    ipcMain.handle('get-volume', async (_) => await service.getVolume());

    createWindow();
})