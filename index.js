import { Platform, NativeModules } from 'react-native'

const ReactNativeMupdf = NativeModules.ReactNativeMupdf;


export default {
  openPdf: (uri, title, options) => {
    const os = Platform.OS.toLowerCase()
    if (os === 'ios') {
      return ReactNativeMupdf.openPdf(uri, title, options, () => {});
    } else if (os === 'android') {
      return ReactNativeMupdf.openPdf(uri, title, options);
    } else {
      throw new Error('unsupported os: ' + os)
    }
  },
};
