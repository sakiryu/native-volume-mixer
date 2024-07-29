const path = require('path');
const CopyWebpackPlugin = require('copy-webpack-plugin');

module.exports = {
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
        }
      ]
    })
  ]
};



/*module.exports = {
  entry: './main.ts',  // Corrected the path based on the location of webpack.config.js
  target: 'electron-main',
  entry: [
    __dirname + "/index.html"
  ],
  module: {
    rules: [
      {
        test: /\.ts$/,
        include: path.resolve(__dirname),  // Include the current directory
        use: [{ loader: 'ts-loader' }]
      },
      {
        test: /\.html/, 
        loader: 'file-loader?name=[name].[ext]'
      }
    ]
  },
  resolve: {
    extensions: ['.ts', '.js']
  },
  output: {
    path: path.resolve(__dirname, 'dist'),  // Ensure the output path is correct
    filename: 'main.js'
  }
};*/
