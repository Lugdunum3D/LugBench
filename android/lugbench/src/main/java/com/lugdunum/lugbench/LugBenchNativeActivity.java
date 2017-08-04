package com.lugdunum.lugbench;


import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.util.HashMap;
import java.util.Map;

public class LugBenchNativeActivity extends NativeActivity {
    private final static String TAG = "LugBenchNativeActivity";
    private String res;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    private String getRequest(final String url) {
        return url;
    }

    private String sendRequest(final String url, final String json) {
        RequestQueue queue = Volley.newRequestQueue(this);

        StringRequest stringRequest = new StringRequest(Request.Method.PUT, url,
            new Response.Listener<String>() {
                @Override
                public void onResponse(String response) {
                    Log.v(TAG, response);
//                    this.res = response;
                    getRequest(response);
                }
            },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Log.v(TAG, "Failed to send the request: " + Integer.toString(error.networkResponse.statusCode));
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
        };

        queue.add(stringRequest);
        return "hey this is sendRequest java response";
    }
}

