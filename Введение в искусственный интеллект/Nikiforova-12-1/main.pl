%Параметры опоры сферической
spherical('2.910.01', m6).
spherical('2.910.02', m8).
spherical('3.910.01', m12).
spherical('3.910.02', m16).

%Параметры опоры с начечкой
notch('2.911.01', m6).
notch('2.911.02', m8).
notch('3.911.01', m12).

%Параметры штыря
pin('2.213.01', 6, 8, m6).
pin('2.213.04', 8, 12, m8).
pin('3.213.06', 12, 26, m12).

%Параметры кулачка с резьбовым отверстием
carving('2.913.05', '30_18', m6, 16).
carving('2.913.06', '45_22', m8, 20).
carving('2.913.09', '65_30', m12, 38).

%Параметры кулачка с призматическими пазами
prismatic('2.913.01', '30_18', 10, 8, 12, 3, 7).
prismatic('2.913.02', '45_22', 12, 8, 12, 3, 7).
prismatic('2.913.07', '65_30', 25, 12, 30, 8, 18).

%Параметры зажима кулачкового
clamp('2.451.01', '45_30', '30_18', 29). clamp('2.451.02', '60_45', '45_22', 34).
clamp('3.451.01', '60_45', '45_22', 35). clamp('3.451.02', '90_60', '65_30', 42).

%Параметры прокладки
gasket('2.217.01', '45_30', 1). gasket('2.217.07', '45_30', 2). gasket('2.217.09', '45_30', 3). gasket('2.217.10', '45_30', 5).
gasket('3.217.01', '60_45', 1). gasket('3.217.07', '60_45', 2). gasket('3.217.09', '60_45', 3). gasket('3.217.10', '60_45', 5).
gasket('3.107.25', '90_60', 2). gasket('3.107.27', '90_60', 3). gasket('3.107.28', '90_60', 5).

device(H, Type, D) :- movable_part(Type, D), clamp_part(), gaskets(H).

/*-------------------------------------*/

movable_part(Type, D) :- mounting_part(Type, D), cam_part(D).

mounting_part(Type, D) :- surface(Type, D).

surface(плоск_чист, _) :- spherical(A, B), b_setval(dm, B), write('Установочный элемент: сферическая опора, код: '), write(A).
surface(плоск_черн, _) :- notch(A, B), b_setval(dm, B), write('Установочный элемент: опора с насечкой, код: '), write(A).
surface(цил_верт, _) :-write('Установочный элемент не требуется.'), b_setval(dm, 'vertical').
surface(цил_гор, _) :- write('Установочный элемент не требуется.'), b_setval(dm, 'horizontal'),!.
surface(перфор, D) :- pin(A, D1, D2, B), D >= D1, D =< D2, b_setval(dm, B), write('Установочный элемент: штырь, код: '), write(A).

cam_part(D) :- b_getval(dm, Dm), cam_type(Dm, D).
cam_type('vertical', D) :- prismatic(A,B,H,_,_,D1,D2), D >= D1, D =< D2, write('\nКулачок: с призматическими пазами, код: '), write(A), b_setval(height, H), b_setval(ssize, B).
cam_type('horizontal', D) :- prismatic(A,B,H,D1,D2,_,_), D >= D1, D =< D2, write('\nКулачок: с призматическими пазами, код: '), write(A), b_setval(height, H), b_setval(ssize, B).
cam_type(A, _) :- carving(Id, B, A, H), write('\nКулачок: с резьбовым отверстием, код: '), write(Id), b_setval(height, H), b_setval(ssize, B), !.

/*-------------------------------------*/

clamp_part() :- b_getval(ssize, S), clamp(A,B,S,H), write('\nЗажим кулачковый, код: '), write(A), b_getval(height, H2), Newheight is H+H2, b_setval(height, Newheight), b_setval(clampsize, B).

/*-------------------------------------*/

gaskets(H) :- write('\nНабор прокладок:'), b_getval(height, Cheight), Cheight2 is H - Cheight - 30,  b_getval(clampsize, Csize), recursivegasket(Cheight2, Csize, 0).

recursivegasket(1,'90_60', _) :- write('\nПодходящих прокладок нет'), !.
recursivegasket(0, _, _) :- write('\nПрокладок не требуется'), !.
recursivegasket(Cheight, _, _) :- Cheight < 0, write('\nВысота конструкции выше запрашиваемой, минимально допустимая высота '), b_getval(height, Cheight2), Out is Cheight2 + 30, write(Out), !.
recursivegasket(6, Csize, N) :- gasket(Id, Csize, 3), gasket(Id2, Csize, 5), write('\n'), write(N), write(' прокладки высотой 5 под кодом '), write(Id2) , write('\n2 прокладки высотой 3 под кодом '), write(Id), !.
recursivegasket(4, Csize, N) :- gasket(Id, Csize, 2), gasket(Id2, Csize, 5), write('\n'), write(N), write(' прокладки высотой 5 под кодом '), write(Id2) , write('\n2 прокладки высотой 2 под кодом '), write(Id), !.
recursivegasket(5, Csize, N) :- gasket(Id, Csize, 5), N2 is N + 1, write('\n'), write(N2), write(' прокладки высотой 5 под кодом '), write(Id), !.
recursivegasket(Cheight, Csize, N) :- gasket(Id, Csize, Cheight), gasket(Id2, Csize, 5), write('\n'), write(N), write(' прокладки высотой 5 под кодом '), write(Id2) , write('\n1 прокладка высотой '), write(Cheight), write(' под кодом '), write(Id), !.
recursivegasket(Cheight, Csize, N) :- gasket(_, Csize, 5), Newheight is Cheight - 5, N2 is N + 1, recursivegasket(Newheight, Csize, N2).
