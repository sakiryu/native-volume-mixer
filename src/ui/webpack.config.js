const path = require('path');
const CopyWebpackPlugin = require('copy-webpack-plugin');

module.exports = {
  mode: 'production',
  entry: {
    main: './main.ts',
    preload: './preload.ts',
    renderer: './renderer.ts'
  },
  target: 'electron-main',
  module: {
    rules: [
      {
        test: /\.ts$/,
        include: path.resolve(__dirname),
        use: [{ loader: 'ts-loader' }]
      }
    ]
  },
  resolve: {
    extensions: ['.ts', '.js']
  },
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: '[name].js'
  },
  plugins: [
    new CopyWebpackPlugin({
      patterns: [
        {
          from: path.resolve(__dirname, 'index.html'),
          to: path.resolve(__dirname, 'dist')
        },
        {
          from: path.resolve(__dirname, 'styles.css'),
          to: path.resolve(__dirname, 'dist'),
        },
      ]
    })
  ],
  stats: {
    all: false,
    errors: true,
    warnings: true,
  },
};