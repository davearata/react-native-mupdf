package com.artifex.mupdfdemo;

import android.content.Context;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import android.net.Uri;
import org.json.JSONException;
import org.json.JSONObject;
import android.content.Intent;
import com.artifex.mupdfdemo.MuPDFActivity;

class ReactNativeMupdfModule extends ReactContextBaseJavaModule {
  private Context context;

  private static WritableMap convertJsonToMap(JSONObject jsonObject) throws JSONException {
    final WritableMap map = new WritableNativeMap();

    final Iterator<String> iterator = jsonObject.keys();
    while (iterator.hasNext()) {
      final String key = iterator.next();
      final Object value = jsonObject.get(key);
      if (value instanceof JSONObject) {
        map.putMap(key, convertJsonToMap((JSONObject) value));
      } else if (value instanceof JSONArray) {
        map.putArray(key, convertJsonToArray((JSONArray) value));
      } else if (value instanceof Boolean) {
        map.putBoolean(key, (Boolean) value);
      } else if (value instanceof Integer) {
        map.putInt(key, (Integer) value);
      } else if (value instanceof Double) {
        map.putDouble(key, (Double) value);
      } else if (value instanceof String) {
        map.putString(key, (String) value);
      } else {
        map.putString(key, value.toString());
      }
    }
    return map;
  }

  public static WritableArray convertJsonToArray(JSONArray jsonArray) throws JSONException {
    final WritableArray array = new WritableNativeArray();

    for (int i = 0; i < jsonArray.length(); i++) {
      final Object value = jsonArray.get(i);
      if (value instanceof JSONObject) {
        array.pushMap(convertJsonToMap((JSONObject) value));
      } else if (value instanceof JSONArray) {
        array.pushArray(convertJsonToArray((JSONArray) value));
      } else if (value instanceof Boolean) {
        array.pushBoolean((Boolean) value);
      } else if (value instanceof Integer) {
        array.pushInt((Integer) value);
      } else if (value instanceof Double) {
        array.pushDouble((Double) value);
      } else if (value instanceof String) {
        array.pushString((String) value);
      } else {
        array.pushString(value.toString());
      }
    }
    return array;
  }

  public ReactNativeMupdfModule(ReactApplicationContext reactContext) {
    super(reactContext);
    this.context = reactContext;
  }

  /**
    * @return the name of this module. This will be the name used to {@code require()} this module
    * from javascript.
    */
  @Override
  public String getName() {
    return "ReactNativeMupdf";
  }

  @ReactMethod
  public void openPdf(String path, String documentTitle, ReadableMap options) {
    final String fileUrl = args.getString(0);
    final String title = args.getString(1);
    final JSONObject options = args.getJSONObject(2);
    final boolean annotationsEnabled = options.getBoolean("annotationsEnabled");
    final boolean isAnnotatedPdf = options.getBoolean("isAnnotatedPdf");
    final String headerColor = options.getString("headerColor");
    final String itemId = options.getString("itemId");

    Uri uri = Uri.parse(fileUrl);

    Intent intent = new Intent(currentActivity.getActivity(), MuPDFActivity.class);

    intent.setAction(Intent.ACTION_VIEW);
    intent.putExtra(MuPDFActivity.KEY_TITLE, title);
    intent.putExtra(MuPDFActivity.KEY_HEADER_COLOR, headerColor);
    intent.putExtra(MuPDFActivity.KEY_ANNOTATIONS_ENABLED, annotationsEnabled);
    intent.putExtra(MuPDFActivity.KEY_IS_ANNOTATED_PDF, isAnnotatedPdf);
    intent.putExtra(MuPDFActivity.KEY_ITEM_ID, itemId)
    intent.setData(uri);

    final Activity currentActivity = getCurrentActivity();
    currentActivity.startActivityForResult(this, intent, 0);
  }

  public void onActivityResult(int requestCode, int resultCode, Intent intent) {
    switch (requestCode) {
    case 0: //integer matching the integer suplied when starting the activity
      if(resultCode == android.app.Activity.RESULT_OK) {
        //in case of success return the string to javascript
        String result = intent.getStringExtra(MuPDFActivity.KEY_SAVE_RESULTS);
        if(result != null && result.length() > 0) {
          try {
            final JSONObject saveResults = new JSONObject(result);
            final WritableMap saveResultsMap = convertJsonToMap(saveResults)
            this.context
              .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
              .emit("PdfSaved", saveResultsMap);
          } catch (JSONException e) {
            e.printStackTrace();
          }
        } else {
          this.context
            .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
            .emit("PdfSaved");
        }
      }
        //  else{
        //      //code launched in case of error
        //      String message = "";
        //      if(intent != null) {
        //          message = intent.getStringExtra("result");
        //      }
        //  }
      break;
    default:
      break;
    }
  }
}
