import React from 'react-native';

const ReactNativeMupdf = React.NativeModules.ReactNativeMupdf;

export default {
  openPdf: () => {
    return ReactNativeMupdf.openPdf();
  },
};
