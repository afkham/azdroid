package com.wso2.azdroidcontroller;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.v4.app.FragmentActivity;
import android.util.Base64;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;


public class MainActivity extends FragmentActivity {

//    public static final String DEFAULT_AZDROID_API = "http://192.168.0.100:9764/Azdroid/services/azdroid_commander/azdroid/move/";

    private static final String CONSUMER_KEY = "29I6fZ2uKrWZ2d_PcMZDQ3qPgU0a";
    private static final String CONSUMER_SECRET = "GOUOqsUIb0wMn1E9h5BPrvD8PzMa";
    //    private static final String DEFAULT_TOKEN_ENDPOINT = "http://192.168.0.100:8080/token";
    private static final String DEFAULT_TOKEN_ENDPOINT = "http://192.168.0.100:8280/token";
    public static final String DEFAULT_AZDROID_API = "http://192.168.0.100:8280/azdroid/1.0/move/";
    public static final String AZDROID_TXT = "azdroid.txt";
    public static final String AZDROID_API_KEY = "azdroid.api";
    public static final String TOKEN_ENDPOINT_KEY = "token.endpoint";

    private String tokenEndpoint = DEFAULT_TOKEN_ENDPOINT;
    private String azdroidAPI = DEFAULT_AZDROID_API;

    private Token accessToken;
    private HttpClient httpClient = new DefaultHttpClient();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        try {
            loadProperties();
        } catch (IOException e) {
            Log.e("cannot.load.props", "Cannot load azdroid.properties file", e);
        }

        findViewById(R.id.buttonForward).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("forward").execute(azdroidAPI);
            }
        });
        findViewById(R.id.buttonReverse).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("reverse").execute(azdroidAPI);
            }
        });
        findViewById(R.id.buttonLeft).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("left").execute(azdroidAPI);
            }
        });
        findViewById(R.id.buttonRight).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("right").execute(azdroidAPI);
            }
        });
        findViewById(R.id.buttonStop).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("stop").execute(azdroidAPI);
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

    private void loadProperties() throws IOException {
        File sdcard = Environment.getExternalStorageDirectory();

        //Get the text file
        File file = new File(sdcard, "Android" + File.separator + "data" + File.separator + getClass().getPackage().getName() + File.separator + AZDROID_TXT);

        Properties properties = new Properties();
        if (file.exists()) {
            try (FileInputStream inputStream = new FileInputStream(file)) {
                properties.load(inputStream);
                this.azdroidAPI = properties.getProperty(AZDROID_API_KEY);
                this.tokenEndpoint = properties.getProperty(TOKEN_ENDPOINT_KEY);
            }
        } else {
            this.azdroidAPI = DEFAULT_AZDROID_API;
            this.tokenEndpoint = DEFAULT_TOKEN_ENDPOINT;

            String dir = sdcard + File.separator + "Android" + File.separator + "data" + File.separator + getClass().getPackage().getName();
            if (new File(dir).mkdirs()) {
                File newFile = new File(dir, AZDROID_TXT);

                properties.put(AZDROID_API_KEY, this.azdroidAPI);
                properties.put(TOKEN_ENDPOINT_KEY, this.tokenEndpoint);
                try (FileOutputStream out = new FileOutputStream(newFile)) {
                    properties.store(out, "Azdroid Controller Configuration");
                }
            }
        }
    }

    private class CallAPITask extends AsyncTask<String, Integer, Boolean> {

        private String direction;

        private CallAPITask(String direction) {
            this.direction = direction;
        }

        protected Boolean doInBackground(String... urls) {
            try {
                if (accessToken == null) {
                    accessToken = getToken();
                }
                if (accessToken != null) {
                    HttpGet request = new HttpGet(urls[0] + direction);
                    request.addHeader("Authorization", "Bearer " + accessToken.getAccessToken());
                    HttpResponse response = httpClient.execute(request);
                    int statusCode = response.getStatusLine().getStatusCode();
                    if (statusCode == HttpStatus.SC_FORBIDDEN) {
                        // Token may have expired. Try to get a new token
                        accessToken = getToken();
                        doInBackground(urls);
                    } else if (statusCode == HttpStatus.SC_OK) {
                        return Boolean.TRUE;
                    }
                }
            } catch (IOException | JSONException e) {
                Log.e("Could not call API", "Could not call API", e);
            }
            return Boolean.FALSE;
        }

        private Token getToken() throws IOException, JSONException {
            // Get a handler that can be used to post to the main thread
            Handler mainHandler = new Handler(MainActivity.this.getMainLooper());
            Runnable accessTokenObtainer = new Runnable() {

                @Override
                public void run() {
                    Toast.makeText(MainActivity.this, "Obtaining access token...", Toast.LENGTH_SHORT).show();
                }
            };
            mainHandler.post(accessTokenObtainer);

            String applicationToken = CONSUMER_KEY + ":" + CONSUMER_SECRET;
            applicationToken = "Basic " + Base64.encodeToString(applicationToken.getBytes("UTF-8"), Base64.DEFAULT);

            HttpPost request = new HttpPost(tokenEndpoint + "?grant_type=client_credentials");
            request.addHeader("Authorization", applicationToken);
            request.addHeader("Content-Type", "application/x-www-form-urlencoded");

            HttpResponse response = httpClient.execute(request);
            int status = response.getStatusLine().getStatusCode();
            if (status == HttpStatus.SC_OK) {
                String result = EntityUtils.toString(response.getEntity());
                return toToken(result);
            }
            return null;
        }

        /**
         * Populates Token object using following JSON String
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
        private Token toToken(String accessTokenJson) throws JSONException {
            Token token = new Token();
            JSONObject jsonObject = new JSONObject(accessTokenJson);
            token.setAccessToken((String) jsonObject.get("access_token"));
            long expiresIn = ((Long) jsonObject.get("expires_in")).intValue();
            token.setExpiresIn(expiresIn);
            token.setRefreshToken((String) jsonObject.get("refresh_token"));
            token.setTokenType((String) jsonObject.get("token_type"));
            return token;
        }
    }
}
