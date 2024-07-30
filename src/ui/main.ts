import { app, ipcMain, BrowserWindow, IpcMainInvokeEvent } from 'electron';
import VolumeMixerService from './volume-mixer-service';
import { spawn } from 'child_process';
import path from 'path';

const spawnVolumeMixerProcess = (): void => {
    const volumeMixerPath: string = path.join(__dirname, '..', '..', 'native', 'x64', 'Debug', 'volume_mixer.exe');

    const volumeMixerProcess = spawn(volumeMixerPath);
    volumeMixerProcess.on('close', (code: number) => {
        console.log(`Child process exited with code ${code}`);
    });

    volumeMixerProcess.on('error', (err: NodeJS.ErrnoException) => {
        console.error('Failed to start child process:', err);
    });
}

const createWindow = (): void => {
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

app.whenReady().then(async (): Promise<void> => {
    spawnVolumeMixerProcess();

    const service = new VolumeMixerService();

    app.on('window-all-closed', async (): Promise<void> => {
        await service.closeApp()
        app.quit();
    });

    ipcMain.handle('set-volume', async (_: IpcMainInvokeEvent, volume: number): Promise<void> =>
        await service.setVolume(volume));

    ipcMain.handle('get-volume', async (_: IpcMainInvokeEvent): Promise<string> =>
        await service.getVolume());

    createWindow();
})