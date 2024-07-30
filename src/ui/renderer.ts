interface VolumeAPI {
    getVolume: () => Promise<string>;
    setVolume: (volume: string) => Promise<void>;
}

interface Window {
    volumeApi: VolumeAPI;
}

const volumeSlider = document.getElementById('volumeSlider') as HTMLInputElement;
const volumeValue = document.getElementById('volumeValue') as HTMLSpanElement;
const muteButton = document.getElementById('muteButton') as HTMLButtonElement;
  
window.onload = async () => {
    const volume = await window.volumeApi.getVolume();
    volumeSlider.value = volume;
    volumeValue.textContent = volume;
};
  
volumeSlider.addEventListener('input', async (event) => {
    const volume = (event.target as HTMLInputElement).value;
    await window.volumeApi.setVolume(volume);
    volumeValue.textContent = await window.volumeApi.getVolume();
});
  
muteButton.addEventListener('click', async () => {
    const volume = '0';
    await window.volumeApi.setVolume(volume);
    volumeSlider.value = volume;
    volumeValue.textContent = volume;
});
  