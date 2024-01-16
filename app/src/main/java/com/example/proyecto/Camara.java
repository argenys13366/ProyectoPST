package com.example.proyecto;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class Camara extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camara);

        Bundle bundle = getIntent().getExtras();
        WebView webView1 = (WebView) findViewById(R.id.webView1);
        WebSettings conf = webView1.getSettings();
        conf.setJavaScriptEnabled(true);
        webView1.setWebViewClient(new WebViewClient());
        webView1.loadUrl("https://www.youtube.com"); // Se reemplaza por el link ip de la SP32CAM
    }
    public void volver(View view) {

        finish();
    }
}