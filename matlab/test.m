
% (J)     moment of inertia of the rotor     0.01 kg.m^2
% (b)     motor viscous friction constant    0.1 N.m.s
% (Ke)    electromotive force constant       0.01 V/rad/sec
% (Kt)    motor torque constant              0.01 N.m/Amp
% (R)     electric resistance                1 Ohm
% (L)     electric inductance                0.5 H

J = 0.01;
b = 0.1;
K = 0.01;
R = 1;
L = 0.5;
s = tf('s');
P_motor = K/((J*s+b)*(L*s+R)+K^2);
step(feedback(P_motor,1))

% we need
% Settling time less than 2 seconds
% Overshoot less than 5%
% Steady-state error less than 1%

pidtool(P_motor)
sisotool()
% get C and use  A/D 0.0001 of arduino
c2d(C,0.0001 ,'tutsin')
% 
%   1616.7 (s+3) (s+9)
%   ------------------
%     (s+90) (s+0.5)

% 
%  1617 s^2 + 1.94e04 s + 4.365e04
%   -------------------------------
%          s^2 + 90.5 s + 45

% 1610 z^2 - 3219 z + 1608
%   ------------------------
%    z^2 - 1.991 z + 0.991
   
%  u(k)=+0.9516*u(k) +5.253e04*e(k)- 5.244e04*e(k-1)
