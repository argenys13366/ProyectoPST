package com.example.proyecto;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.airbnb.lottie.LottieAnimationView;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.HashMap;

public class Ruido extends AppCompatActivity {
    TextView temp;
    Button buzzer;
    LottieAnimationView animationView;
    boolean estado1 = false;
    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference referenceEntrada1 = database.getReference("LECTURAS");
    DatabaseReference getReferenceEntrada2 = database.getReference("BUZZER");


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ruido);
        temp = findViewById(R.id.temp);
        buzzer = findViewById(R.id.buzzer);
        animationView = findViewById(R.id.animationView);

        referenceEntrada1.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                String T=dataSnapshot.child("temp").getValue().toString();
                temp.setText("Temperatura: "+T+"°C");
                animationView.playAnimation();

            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

        getReferenceEntrada2.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                String estadoBuzzer = dataSnapshot.child("Valor").getValue().toString();

            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
        buzzer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                HashMap<Object, Object> info = new HashMap<>();
                estado1 =! estado1;
                info.put("Valor", estado1);
                getReferenceEntrada2.setValue(info);
            }
        });
    }

    public void volver(View view) {

        finish();
    }
}