import React from 'react-native';

const ReactNativeMupdf = React.NativeModules.ReactNativeMupdf;

export default {
  openPdf: (onSuccess, onFailure) => {
    return ReactNativeMupdf.openPdf(onSuccess, onFailure);
  },
};
