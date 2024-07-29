const PipeChannel = require('./pipe-channel');

class VolumeMixerService {
    constructor() {
        this._pipe = new PipeChannel('VolumeMixer');
    }

    async closeApp() {
        await this._pipe.write('/close_app');
    }

    async setVolume(volume) {
        await this._pipe.write(`/set_volume ${volume.toString()}`);
    }

    async getVolume() {
        await this._pipe.write('/get_volume');
        return await this._pipe.read();
    }
}
module.exports = VolumeMixerService;
