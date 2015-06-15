package com.wso2.azdroidcontroller;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;


public class MainActivity extends FragmentActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.buttonForward).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                new CallAPITask("forward").execute("http://wso2.com");
            }
        });
        findViewById(R.id.buttonReverse).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(), "Moving backward...", Toast.LENGTH_SHORT).show();

            }
        });
        findViewById(R.id.buttonLeft).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(), "Turning left...", Toast.LENGTH_SHORT).show();

            }
        });
        findViewById(R.id.buttonRight).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(), "Turning right...", Toast.LENGTH_SHORT).show();

            }
        });
        findViewById(R.id.buttonStop).setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(), "Stopping...", Toast.LENGTH_SHORT).show();

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
            HttpGet request = null;
            try {
                request = new HttpGet(urls[0]);
                ResponseHandler<String> handler = new BasicResponseHandler();
                String result = httpclient.execute(request, handler);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return Boolean.TRUE;
        }
    }
}
