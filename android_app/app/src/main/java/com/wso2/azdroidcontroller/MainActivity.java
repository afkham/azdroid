package com.wso2.azdroidcontroller;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import org.apache.http.client.HttpClient;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.IOException;


public class MainActivity extends FragmentActivity {

//    public static final String AZDROID_API = "http://192.168.0.100:9764/Azdroid/services/azdroid_commander/azdroid/move/";
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
                String result = httpclient.execute(request, handler);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return Boolean.TRUE;
        }
    }
}
