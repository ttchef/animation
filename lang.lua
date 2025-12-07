shape ttchef {
    pos = (90, 30)
    easing = .lerp

    fn dob {
        pos = (100, 90)
    }

    fn bob {
        easing = .bezier
        pos = (1000, 90)
    }
}

shape harald {

}

timeline {
    10-20 harald.visible = 9;
    70-90 call ttchef.dob
    70-90 call ttchef.dob
    90-100 call harald.carAccident
}


