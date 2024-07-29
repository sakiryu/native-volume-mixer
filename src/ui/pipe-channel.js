const net = require('net');

class PipeChannel {
    constructor(pipeName) {
        this._client = this._connect(`\\\\.\\pipe\\${pipeName}`);
    }

    _connect(pipeName, maxAttempts = 5) {
        return new Promise((resolve, reject) => {
            let attempts = 0;

            const tryConnect = () => {
                attempts++;
                const client = net.createConnection(pipeName, () => {
                    clearInterval(connectionInterval);
                    resolve(client);
                });

                client.on('error', (err) => {
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

    async write(message) {
        const client = await this._client;

        return new Promise((resolve, reject) => {
            client.write(message, (err) => {
                if (err) {
                    reject(err);
                    return;
                }
                resolve();
            });
        });
    }

    async read() {
        const client = await this._client;

        return new Promise((resolve, reject) => {
            const onData = (data) => {
                cleanup();
                resolve(data.toString('utf8').trim());
            };

            const onError = (err) => {
                cleanup();
                reject(err);
            };

            const cleanup = () => {
                client.removeListener('data', onData);
                client.removeListener('error', onError);
            };

            client.once('data', onData);
            client.once('error', onError);
        });
    }
}
module.exports = PipeChannel;