import React from 'react-native';

const ReactNativeMupdf = React.NativeModules.ReactNativeMupdf;

export default {
  openPdf: (uri, title, options) => {
    return ReactNativeMupdf.openPdf(uri, title, options);
  }
};
