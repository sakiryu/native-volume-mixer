import { contextBridge, ipcRenderer } from 'electron';

contextBridge.exposeInMainWorld('electronAPI', {
    setVolume: (volume: number) => ipcRenderer.invoke('set-volume', volume),
    getVolume: () => ipcRenderer.invoke('get-volume')
});
