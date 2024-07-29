import PipeChannel from './pipe-channel';

export default class VolumeMixerService {
    private _pipe: PipeChannel;
    
    constructor() {
        this._pipe = new PipeChannel('VolumeMixer');
    }

    async closeApp(): Promise<void> {
        await this._pipe.write('/close_app');
    }

    async setVolume(volume: number): Promise<void> {
        await this._pipe.write(`/set_volume ${volume.toString()}`);
    }

    async getVolume() {
        await this._pipe.write('/get_volume');
        return await this._pipe.read();
    }
}