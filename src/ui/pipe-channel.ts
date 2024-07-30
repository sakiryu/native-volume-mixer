import net, { Socket } from 'net';

export default class PipeChannel {
    private _client: Promise<any>;

    constructor(pipeName: string) {
        this._client = this._connect(`\\\\.\\pipe\\${pipeName}`);
    }

    _connect(pipeName: string, maxAttempts: number = 5): Promise<Socket> {
        return new Promise<Socket>((resolve, reject) => {
            let attempts = 0;

            const tryConnect = (): void => {
                attempts++;
                const client = net.createConnection(pipeName, () => {
                    clearInterval(connectionInterval);
                    resolve(client);
                });

                client.on('error', (err: NodeJS.ErrnoException): void => {
                    if (err.code === 'ENOENT') {
                        console.error('Pipe not found. Retrying...');
                        if (attempts >= maxAttempts) {
                            clearInterval(connectionInterval);
                            reject(new Error(`Failed to connect to the pipe after ${maxAttempts} attempts.`));
                        }
                    } else {
                        clearInterval(connectionInterval);
                        reject(err);
                    }
                });
            };

            const connectionInterval = setInterval(tryConnect, 1000);
        });
    }

    async write(message: string): Promise<void> {
        const client = await this._client;

        return new Promise<void>((resolve, reject) => {
            client.write(message, (err: NodeJS.ErrnoException) => {
                if (err) {
                    reject(err);
                    return;
                }
                resolve();
            });
        });
    }

    async read(): Promise<string> {
        const client = await this._client;

        return new Promise<string>((resolve, reject) => {
            const onData = (data: string) => {
                cleanup();
                resolve(data.toString().trim());
            };

            const onError = (err: NodeJS.ErrnoException): void => {
                cleanup();
                reject(err);
            };

            const cleanup = (): void => {
                client.removeListener('data', onData);
                client.removeListener('error', onError);
            };

            client.once('data', onData);
            client.once('error', onError);
        });
    }
}