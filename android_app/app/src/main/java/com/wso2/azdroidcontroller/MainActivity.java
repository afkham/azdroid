package com.wso2.azdroidcontroller;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.util.Base64;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class MainActivity extends FragmentActivity {

//    public static final String AZDROID_API = "http://192.168.0.100:9764/Azdroid/services/azdroid_commander/azdroid/move/";

    private static final String CONSUMER_KEY = "29I6fZ2uKrWZ2d_PcMZDQ3qPgU0a";
    private static final String CONSUMER_SECRET = "GOUOqsUIb0wMn1E9h5BPrvD8PzMa";
    private static final String TOKEN_ENDPOINT = "http://192.168.0.100:8280/token";
    public static final String AZDROID_API = "http://192.168.0.100:8280/azdroid/1.0/move/";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.buttonForward).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("forward").execute(AZDROID_API);
            }
        });
        findViewById(R.id.buttonReverse).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("reverse").execute(AZDROID_API);
            }
        });
        findViewById(R.id.buttonLeft).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("left").execute(AZDROID_API);
            }
        });
        findViewById(R.id.buttonRight).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("right").execute(AZDROID_API);
            }
        });
        findViewById(R.id.buttonStop).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("stop").execute(AZDROID_API);
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private class CallAPITask extends AsyncTask<String, Integer, Boolean> {

        private String direction;

        private CallAPITask(String direction) {
            this.direction = direction;
        }

        protected Boolean doInBackground(String... urls) {
            HttpClient httpclient = new DefaultHttpClient();
            HttpGet request;
            try {
                request = new HttpGet(urls[0] + direction);
                request.addHeader("Authorization", "Bearer a27cd3fbc81fdb2fa2161837a5165dae");
                ResponseHandler<String> handler = new BasicResponseHandler();
                String response = httpclient.execute(request, handler);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return Boolean.TRUE;
        }

        private Token getToken() {
            HttpClient httpClient = new DefaultHttpClient();
            try {
                String applicationToken = CONSUMER_KEY + ":" + CONSUMER_SECRET;
                applicationToken = "Basic " + Base64.encodeToString(applicationToken.getBytes("UTF-8"), Base64.DEFAULT);

                HttpPost request = new HttpPost(TOKEN_ENDPOINT);
                request.addHeader("Authorization", applicationToken);
                request.addHeader("Content-Type", "application/x-www-form-urlencoded");
                List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(1);
                nameValuePairs.add(new BasicNameValuePair("grant_type", "client_credentials"));
                request.setEntity(new UrlEncodedFormEntity(nameValuePairs));

                ResponseHandler<String> handler = new BasicResponseHandler();
                String response = httpClient.execute(request, handler);
                return getAccessToken(response);
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
        }

        /**
         * Populates Token object using folloing JSON String
         * {
         * "token_type": "bearer",
         * "expires_in": 3600000,
         * "refresh_token": "f43de118a489d56c3b3b7ba77a1549e",
         * "access_token": "269becaec9b8b292906b3f9e69b5a9"
         * }
         *
         * @param accessTokenJson
         * @return
         */
        private Token getAccessToken(String accessTokenJson) {
            Token token = new Token();
            try {
                JSONObject jsonObject = new JSONObject(accessTokenJson);
                token.setAccessToken((String) jsonObject.get("access_token"));
                long expiresIn = ((Long) jsonObject.get("expires_in")).intValue();
                token.setExpiresIn(expiresIn);
                token.setRefreshToken((String) jsonObject.get("refresh_token"));
                token.setTokenType((String) jsonObject.get("token_type"));
            } catch (JSONException e) {
                e.printStackTrace();
            }
            return token;
        }
    }
}
