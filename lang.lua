0s {
    Camera.pos = 20.0;
    Scene.color = "FFFFFF";
}

10s {
    10s new Hello;
    Hello.model = "lucas";
    Hello.easing = .lerp;
    20s Hello.opacity = 0.1;
    20s - 40s Hello.pos = 10;
    
}

10s {
    new Lucas;
    20s Lucas.pos = 20;
}
