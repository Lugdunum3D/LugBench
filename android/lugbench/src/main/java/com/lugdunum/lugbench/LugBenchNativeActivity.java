package com.lugdunum.lugbench;


import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;

import com.android.volley.AuthFailureError;
import com.android.volley.DefaultRetryPolicy;
import com.android.volley.NetworkError;
import com.android.volley.NetworkResponse;
import com.android.volley.NoConnectionError;
import com.android.volley.ParseError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.RetryPolicy;
import com.android.volley.ServerError;
import com.android.volley.TimeoutError;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.Map;

public class LugBenchNativeActivity extends NativeActivity {
    private final static String TAG = "LugBenchNativeActivity";
    private RequestQueue queue;
    private String mLastRequestBody;
    private int mLastRequestStatusCode;
    private String mUrl;
    private String mJson;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        queue = Volley.newRequestQueue(this);
        mLastRequestStatusCode = 0;
    }

    private void put(final String url, final String json) {
        mUrl = url;
        mJson = json;

        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
            new Response.Listener<String>() {
                @Override
                public void onResponse(String response) {
                    nativeCallback(mLastRequestStatusCode, response);
                }
            },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    if (error instanceof NetworkError || error instanceof NoConnectionError) {
                        Log.v(TAG, "No internet...");
                        put(mUrl, mJson);
                    }
                    if (error == null || error.networkResponse == null) {
                        Log.v(TAG, "Failed to send");
                        return;
                    }
                    if (error.networkResponse.statusCode == 409) {
                        mLastRequestStatusCode = error.networkResponse.statusCode;
                        mLastRequestBody = new String(error.networkResponse.data);
                        nativeCallback(error.networkResponse.statusCode, mLastRequestBody);
                        return;
                    }
                    Log.v(TAG, "Unhandled Status Code " + Integer.toString(error.networkResponse.statusCode));
                }
            }
        ) {
            @Override
            public byte[] getBody() throws AuthFailureError {
                return json.getBytes();
            }
            @Override
            public Map<String, String> getHeaders() throws AuthFailureError {
                Map<String, String> headers = new HashMap<>();
                headers.put("Content-Type","application/json");
                headers.put("User-agent", "LugBench/0.1.0");
                return headers;
            }
            @Override
            protected Response<String> parseNetworkResponse(NetworkResponse response) {
                mLastRequestStatusCode = response.statusCode;
                Log.v(TAG, Integer.toString(response.statusCode));
                return super.parseNetworkResponse(response);
            }

        };

        stringRequest.setRetryPolicy(new DefaultRetryPolicy(
                3000,   //DefaultRetryPolicy.DEFAULT_TIMEOUT_MS,
                3000,   //DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                DefaultRetryPolicy.DEFAULT_BACKOFF_MULT));

        queue.add(stringRequest);
    }

    public native void nativeCallback(int statusCode, String body);
    static {
        System.loadLibrary("lugbench");
    }
}
