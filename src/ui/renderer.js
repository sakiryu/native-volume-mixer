const volumeSlider = document.getElementById('volumeSlider');
const volumeValue = document.getElementById('volumeValue');
const muteButton = document.getElementById('muteButton');

window.onload = async () => {
    const volume = await window.electronAPI.getVolume();
    volumeSlider.value = volume;
    volumeValue.textContent = volume;
};

volumeSlider.addEventListener('input', async (event) => {
    const volume = event.target.value;
    await window.electronAPI.setVolume(volume);

    volumeValue.textContent = await window.electronAPI.getVolume();
});

muteButton.addEventListener('click', async () => {
    const volume = 0;
    await window.electronAPI.setVolume(volume);
    volumeSlider.value = volume;
    volumeValue.textContent = volume;
});