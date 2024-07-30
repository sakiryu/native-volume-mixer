import { app, ipcMain, BrowserWindow, IpcMainInvokeEvent } from 'electron';
import VolumeMixerService from './volume-mixer-service';
import { spawn } from 'child_process';
import path from 'path';

const spawnVolumeMixerProcess = (nativeVolumeMixerPath: string): void => {
    const volumeMixerProcess = spawn(nativeVolumeMixerPath);
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
    
    const nativeVolumeMixerPath: string = process.argv.slice(2)[0];
    if(!nativeVolumeMixerPath) {
        console.error('Path to volume mixer was not provided.')
        app.quit();
    }
    
    spawnVolumeMixerProcess(nativeVolumeMixerPath);

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