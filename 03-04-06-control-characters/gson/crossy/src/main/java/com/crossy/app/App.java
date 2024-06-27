package com.crossy.app;

import java.nio.charset.StandardCharsets;

public class App 
{
    public static void main(String[] args)
    {
        GsonTarget gsonTarget = new GsonTarget();
        String templateString = "[\"0\"]";

        for (byte i = 0x00; i <= 0x1f; i++) {
            byte[] jsonBytes = templateString.getBytes(StandardCharsets.UTF_8);
            jsonBytes[2] = i;

            byte[] result = gsonTarget.run(jsonBytes);
            System.out.println(jsonBytes[2]);
            System.out.println(new String(result, StandardCharsets.UTF_8));
        }
    }
}
