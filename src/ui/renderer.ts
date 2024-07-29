interface ElectronAPI {
    getVolume: () => Promise<string>;
    setVolume: (volume: string) => Promise<void>;
}

interface Window {
    electronAPI: ElectronAPI;
}

const volumeSlider = document.getElementById('volumeSlider') as HTMLInputElement;
const volumeValue = document.getElementById('volumeValue') as HTMLSpanElement;
const muteButton = document.getElementById('muteButton') as HTMLButtonElement;
  
window.onload = async () => {
    const volume = await window.electronAPI.getVolume();
    volumeSlider.value = volume;
    volumeValue.textContent = volume;
};
  
volumeSlider.addEventListener('input', async (event) => {
    const volume = (event.target as HTMLInputElement).value;
    await window.electronAPI.setVolume(volume);
    volumeValue.textContent = await window.electronAPI.getVolume();
});
  
muteButton.addEventListener('click', async () => {
    const volume = '0';
    await window.electronAPI.setVolume(volume);
    volumeSlider.value = volume;
    volumeValue.textContent = volume;
});
  