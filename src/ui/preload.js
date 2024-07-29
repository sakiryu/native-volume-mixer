const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
    setVolume: (volume) => ipcRenderer.invoke('set-volume', volume),
    getVolume: () => ipcRenderer.invoke('get-volume')
});
