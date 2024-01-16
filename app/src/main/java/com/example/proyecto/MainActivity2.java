package com.example.proyecto;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class MainActivity2 extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);
    }
    public void iniciocamara(View view) {
        Intent i = new Intent(this, Camara.class );
        startActivity(i);
    }
    public void inicioruido(View view) {
        Intent i = new Intent(this, Ruido.class );
        startActivity(i);
    }

    public void volver(View view) {

        finish();
    }
}