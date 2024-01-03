package com.yooha.antisdk;

import android.content.Intent;
import android.os.Bundle;
import android.os.storage.StorageManager;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class PassActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pass);

        Intent intent = getIntent();
        String value = intent.getStringExtra("value");

        TextView tv = findViewById(R.id.textView);
        tv.setText(value);

//        mEtx = findViewById(R.id.editTextTextPassword);
//        mBtn = findViewById(R.id.button);
//        mBtn.setOnClickListener(new View.OnClickListener(){
//
//            @Override
//            public void onClick(View v) {
//                String key = mEtx.getText().toString();
//                Log.i("yoohaa", key);
//            }
//        });

    }

}
