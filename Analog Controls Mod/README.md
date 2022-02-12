Turok, Armorines, Rainbow Six Control, Duke, Forsaken, Daikatana, Shadowgate, Hexen, Kirby 64, Yoshi's Story, Mischief Makers, Bomberman 64 Arcade, Super Smash Bros, Jet Force Gemini and Mortal Kombat Mythologies Modification
by SubDrag
Last Updated 7/21/2017
Special thanks to Zoinkity for his assistance in dead zone, finding space in Daikatana to replace the MPAL/PAL video modes, and general testing/assembly assistance.

Turok was a game I always wanted to play through, but the "PC-like" controls on an N64 just never quite did it for me.  I really enjoyed Turok 2, and even Turok 3, but never was able to really do the original.  I never made it that far due to the controls, but I always wanted to play through this game.  Much later I tried Armorines, and it seemed even more awkward, and I never got anywhere. I'm still not clear why Armorines didn't have an option for analog movement controls, considering it had 5 control schemes, though oddly none right.

I always hoped someone would make a patch to make the controls be more like GoldenEye, but alas, no one ever had.  I had considered the emulator option, but my adapters never really quite worked right, and nothing like just playing on the N64 itself, and it always felt different.  Also, even when swapping c-up/down and analog up/down, without auto-centering, it would've worked, but wasn't quite good enough.  Turok Remastered, that came out on Steam, was not really compatible with the N64 adapter properly, and even if it had, I don't think it would've had the right feel for me, as I wanted to have it feel like N64 (graphics, control, etc), pea-soup fog and save points and all.  So I finally tried to give it a final attempt myself, to "fix" the controls, and with decent effort, and quite a bit of ASM rewrites, I finally had surprising success.

So essentially, these patches make the controls the more standard analog to turn and move forward/back, and c-up/c-down to aim up and down.  The first part of the hack was to essentially swap the c-up/down and analog up/down, and also make it so the aim up/down was incremental (essentially converting to Look Spring, as it was not in Turok that way).  Also, to better mimic the way movement works, it auto-centers the vertical when go forward or backwards (from a lull), while can aim at a run without it inadvertantly auto-centering.  It is very similar to the way most fps work on N64, such as GoldenEye (without the R to aim, it jumps instead).  I think the controls ended up pretty slick, and even though the game once in a while feels a little "slippery", I believe that is due to the way the controls work in the original game.  

The horizontal and vertical sensitivities work for both games, which is helpful, and it is recommended to tweak them both until you are satisfied with your movement, aiming, and turning.  Armorines works for all players, including cooperative and multiplayer modes.  Do not attempt to change LookSpring or Control Style (they have been disabled, but if you find a way to do it, don't do it).  The modification is disabled in the menus, so should not interfere with them.  Also, holding A will manually recenter the view (although it also has auto-center), and toggling analog stick up/down will change to special weapons, and left/right to normal ones, same as how the game was before for that.

I added Tom Clancy's Rainbow Six as well, since it also uses Turok controls This one was a bit more unique, as it had other controls that did not resemble what I was used to, so I've changed the button scheme as well.  It also had more buttons needed than fit perfectly to GoldenEye's, so I had to make some adjustments (Controller Scheme 2 is not touched).  I left A as run, because if it were L or d-pad, it would be too difficult to use.  Y can be inverted in the menu.  Also, remember that while aiming (holding R), it will be analog stick to move, much like GoldenEye.

Jet Force Gemini Inverts (to Non-Invert), when pressing the R Button, for all players, and the Target Range in multi.  Note that when choosing Target range, the menus will be inverted.

A No Invert patch is provided for all of the games where Invert cannot be done already by an ingame menu.

Tom Clancy's Rainbow Six Original (Controller Scheme 1):
B - Change Weapon
A - Zoom
L - Change Magazine
R - Run
D-Pad Up/Right/Down - Action

Tom Clancy's Rainbow Six (Controller Scheme 1):
L or D-Right - Change Weapon
R - Zoom
B - Change Magazine
A - Run
D-Pad Up/Down - Action

Hexen luckily already had analog controls, the patch just remaps the controls to a more standard N64 control scheme. Unfortunately, there is still no strafing, as not present in the original game.

New Controls
R Jump
Z Attack
A/C-Left or C-right, Change Item
D-left/D-right Change Weapon
L Map
C-up/C-down Look up/down (inverted)
C-left and stick/left right, to strafe
A/D-up or D-down, Fly (when can)
A/C-down Use Item

For Duke Nukem 64, in control style "Control Stick Move" only:
C-up/down to aim (inverted)
R Jump
A Crouch
D-Down Weapon Fwd
D-Pad up Weapon Back
B Use

Also, added, in that same control style, it now auto-centers with a big tug of the stick from a lull (and an option to not do that).

Forsaken, I could not figure out how to disable the control mods in menus, so you must use either C or D-pad in menus to navigate, not the control stick.  Do not change the default control option from 4 in Forsaken or the mod will not work properly. Other than swapping C-up/down and D-pad up/down, there are no further changes.

Daikatana, the C and Analog up/down are swapped. Only the Reverse or Normal Free control schemes should be used, do not selected the Fixed control schemes or they will not work correctly.  Use the sensitivity in the menu to adjust how fast up/down aim using C-buttons changes it.

Shadowgate 64 the C and Analog up/down are swapped, and the weird can only go up or down, and must recenter, has been nullified.  As of now, strafe + move sadly has not been fixed, hopefully later.

For Mortal Kombat Mythologies, there were some key fixes, to automatically face the closest enemy when punching and kicking.  Previously, this only occurred when jumping, now also when punching and kicking which helps minimize the amount of manual switch directions.  This makes a pretty nice difference in controls.

Also, for simplicity, the default controls have been modified to be close to the ones used by Mortal Kombat Trilogy. This could've been done already, since the game let you adjust the controls in the menu.

Unfortunately not all moves auto-switch sides in this patch, such as crouch moves.  When I tried the crouch versions, it stands up to punch for a couple frames and then switches before the normal move.  I guess the algorithm they hadn't only supported this for standing and jumping, so I removed this.  Somehow, this needs to be improved later to not cause the extra punch.  This patch still helps quite a bit in playability.

C-Left	Block
C-Down	Run
C-Up	High Kick
C-Right	Low Kick
L	Use/Action
R	Switch Direction
A	Low Punch
B	High Punch
START	Pause
Z	Inventory
	
N64 Elevators:	
(Hold L), C-Up:  Up	
(Hold L), C-Up:  Down	

Kirby 64 simply copies Analog inputs into the D-Pad inputs (so essentially allows the user to play using either analog, or D-Pad)

Yoshi's Story converts the increase/decrease fruit border size to C-up/C-down, and then lets you use the D-Pad.  Note that since there is only D-buttons on or off, there is no way to slow crawl (you can still use the analog stick if that is required at a point).

Mischief Makers simply copies Analog inputs into the D-Pad inputs (so essentially allows the user to play using either analog, or D-Pad)
Bomberman 64 Arcade simply copies Analog inputs into the D-Pad inputs (so essentially allows the user to play using either analog, or D-Pad)
Super Smash Bros simply copies Analog inputs into the D-Pad inputs (so essentially allows the user to play using either analog, or D-Pad).  (Version 1.0 and 1.01)
Version 1.01 also makes the L button the same as Z (block), and C-Up is now Taunt.

Patches intended only for these versions:
Armorines - Project S.W.A.R.M. (U)
Turok - Dinosaur Hunter (U) (V1.0)
Tom Clancy's Rainbow Six (U)
Hexen (U)
Duke Nukem 64 (U)
Forsaken 64 (U)
John Romero's Daikatana (U)
Shadowgate 64 - Trials Of The Four Towers (U) (M2)
Mortal Kombat Mythologies - Sub-Zero (U)
Kirby 64 - The Crystal Shards (U)
Yoshi's Story (U) (M2) [!]
Mischief Makers (U) [!]
Bomberman 64 - Arcade Edition (J)
Bomberman 64 - Arcade Edition (J) [Zoinkity's Translation]
Super Smash Bros. (U) [!]

To Patch, use the latest version of the GoldenEye Setup Editor (Tools -> XDelta Patching -> Apply XDelta Patch (any game), or your favorite xDelta patcher.  To use the GoldenEye Setup Editor, open it up in any mode, Tools->XDelta Patching->Apply XDelta Patch (any game).  Then load a clean, unzipped ROM of Turok 1.0 US for example, then the patch, then your output ROM name (must not equal input name).

If you have any issues, questions, ideas, or comments, send me an email at:
SubDrag@goldeneyevault.com

Or visit my web site at:
http://www.goldeneyevault.com/





Source included for reference below.  Note that Turok file must be compressed, as the assembly code is stored in a compressed RNC1 file.  Armorines does not, though be aware that the assembly code is executed in TLB mapped to 002XXXXX address space.  It should be able to be ported to other versions with only minor modifications for addresses, but I have not done so.  Both mods replace debug text, that is already preloaded/copied from ROM to RAM. 

SOURCE:

Turok US 1.0 NTSC

File 783690 has compressed original at 0x0000fa84

80010D84:
LUI $at, 800F
ori $at, $at, 0CC0
jr $at
nop

3C01800F
34210CC0
00200008
00000000



CHANGE TO REPLACE DEBUG TEXT at 800F0CC0, file 783690 at EF9C0

Store byte at 800F0DE0 of current aim value
Store byte at 800F0DE1 is previous actual analog vertical


800F0CC0:
LWL     $at, 0x0000($t8)
LWR     $at, 0x0003($t8)

; If menu, value is 1, so skip special logic 801196eb
lui $a0, 8012
lb $a0, 0x96eb($a0)

bne $a0, $zero, End
nop

lui $a0, 0008
lui $t9, 0004

and $t6, $at, $a0
and $t7, $at, $t9

lb $t4, 0x0003($t8)
; absolute value
sra $t5,$t4,0x1F
xor $t4,$t4,$t5
sub $t4,$t4,$t5

andi $t5, $at, 0080

lui $t3, FFF3
ori $t3, $t3, FF00

and $at, $at, $t3

lui $t1, 800F
lb $t2, 0DE0($t1)

; old
lbu $v1, 0DE1($t1)

; sort of controller turn, try and avoid auto-center there
addi $v1, $v1, 0x10

; if new one is bigger
slt $v1, $v1, $t4

; if new smaller
beq $v1, zero, EndNewAnalogCheck
addiu $v0, $zero, 0

; is new bigger analog
addiu $v0, $zero, 1

EndNewAnalogCheck:
; Dead zone check, don't detect and auto-recenter if dead zone
sltiu $v1, $t4, 0x10

sb $t4, 0DE1($t1)

beql $t6, $a0, NoCUp
addi $t2, $t2, 2

NoCUp:
beql $t7, $t9, NoCDown
addi $t2, $t2, 0xFFFE

NoCDown:
slti $t0, $t2, 0xFFBB
bnel $t0, $zero, LessThanMax
addiu $t2, $zero, 0x00BB

LessThanMin:
slti $t0, $t2, 0x0046
bne $t0, $zero, LessThanMax
nop
addiu $t2, $zero, 0x0045

LessThanMax:
andi $t2, $t2, FF

; Dead zone check, don't detect and auto-recenter if dead zone
bnel $v1, zero, NoFwd
nop

beq $t5, $zero, NoBack
nop
or $at, $at, $t9

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoBack:
or $at, $at, $a0

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoFwd:

sb $t2, 0DE0($t1)
or $at, $at, $t2

End:
LUI     $v1, 0x8012
ADDIU   $v1, $v1, 0x9918
j 80010D94
nop





























ARMORINES US
800F7550 is controller activator spot


LookSpring force On
Single
8011599F 0001

Co-op/Multi
801159E1 0001 P1
80115A23 0001 P2
80115A61 0001 P3
80115AA7 0001 P4

Force Control Scheme Normal
Single
8011599D 0000

Co-op/Multi
801159DF 0000 P1
80115A23 0000 P2
80115A61 0000 P3
80115AA7 0000 P4

ASM to force on
00418E48 NOP
004187B0 SB S3, 0x002F($A0)  A093002F
004187FC SB S0, 0x002F($A0)  A090002F

00418C8C NOP



@ROM 0008AAEC

80089EEC:
LUI $t0, 002A
ori $t0, $t0, B750
jr $t0
nop



CHANGE TO REPLACE DEBUG TEXT at RAM TLB 800AB750

Store byte at 800AB8D0 of current aim value
Store byte at 800AB8D4 is previous actual analog vertical
+S3 * # Players (which is 0,6,C,12)


@ROM 000AC350

800AB750:
; If menu, value is 1, so skip special logic 80115b0b
lui $a0, 8011
lb $a0, 0x5b0b($a0)

bne $a0, $zero, End
LUI     $t0, 0x800F


ADDU    $t0, $t0, $s3
LWL     $at, 0x7550($t0)
LWR     $at, 0x7553($t0)

; Check A pressed
lui $t9, 8000
and $t9, $at, $t9

;load signed analog up/down
lb $t4, 0x7553($t0)

; In case exits out, store old analog to there, also loaded here now
lui $t1, 800F
ADDU    $t1, $t1, $s3
lb $t2, 0870($t1)
lbu $v1, 0874($t1)

; Store 0 in case A pressed
sb $zero, 0870($t1)

bne $t9, $zero, End
lui $a0, 0008


lui $t9, 0004

and $t6, $at, $a0
and $t7, $at, $t9


; absolute value
sra $t5,$t4,0x1F
xor $t4,$t4,$t5
sub $t4,$t4,$t5
andi $t5, $at, 0080

lui $t3, FFF3
ori $t3, $t3, FF00

and $at, $at, $t3





; sort of controller turn, try and avoid auto-center there
addi $v1, $v1, 0x10

; if new one is bigger
slt $v1, $v1, $t4

; if new smaller
beq $v1, zero, EndNewAnalogCheck
addiu $v0, $zero, 0

addiu $v0, $zero, 1


EndNewAnalogCheck:
; Dead zone check, don't detect and auto-recenter if dead zone
sltiu $v1, $t4, 0x10

sb $t4, 0874($t1)

beql $t6, $a0, NoCUp
addi $t2, $t2, 2

NoCUp:
beql $t7, $t9, NoCDown
addi $t2, $t2, 0xFFFE

NoCDown:
slti $a1, $t2, 0xFFBB
bnel $a1, $zero, LessThanMax
addiu $t2, $zero, 0x00BB

LessThanMin:
slti $a1, $t2, 0x0046
bne $a1, $zero, LessThanMax
nop
addiu $t2, $zero, 0x0045

LessThanMax:
andi $t2, $t2, FF

; Dead zone check, don't detect and auto-recenter if dead zone
bnel $v1, zero, NoFwd
nop

beq $t5, $zero, NoBack
nop
or $at, $at, $t9

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoBack:
or $at, $at, $a0

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoFwd:

sb $t2, 0870($t1)
or $at, $at, $t2

SWL     $at, 0x7550($t0)
SWR     $at, 0x7553($t0)

End:
LUI     $a0, 0x800F
ADDIU   $a0, $a0, 0x77A0

j 00289EFC
ADDU    $at, $zero, $t0









Rainbow Six US

Controller Scheme 1
80081746 B Spot 0x4000
8008174C A Spot 0x8000
80081752 L Spot 0x0020
80081758 R Spot 0x0010
8008176A D-Pad Up/Right/Down 0x0D00

Original:
ROM:00082346 4000 B - Change Weapon
ROM:0008234C 8000 A - Zoom
ROM:00082352 0020 L - Change Magazine
ROM:00082358 0010 R- Run
ROM:0008236A 0D00 D-Pad Up/Right/Down - Action

New:
ROM:00082346 0120 L or D-Right - Change Weapon
ROM:0008234C 0010 R - Zoom
ROM:00082352 4000 B - Change Magazine
ROM:00082358 8000 A - Run
ROM:0008236A 0C00 D-Pad Up/Down - Action


Controller Scheme 2 is after this, but unchanged
ROM:000823E8:8000
ROM:000823F4:0010






;A2,A3,T0,T1 Unsafe
;A0,A1 OK

ROM: 000718BC


80070CBC:
LWL     $v0, 0x0000($t0)
LWR     $v0, 0x0003($t0)
SWL     $v0, 0x0000($sp)
SWR     $v0, 0x0003($sp)
LUI $t2, 8008
ori $t2, $t2, BD18
jr $t2
nop




ROM: 0008C918



8008BD18:
LWL     $at, 0x0004($t0)
LWR     $at, 0x0007($t0)

; If menu, value is 1, so skip special logic 800b3c40
lui $a0, 800b
lb $a0, 0x3c40($a0)

bne $a0, $zero, End
nop

lui $a0, 0008
lui $t9, 0004

and $t6, $at, $a0
and $t7, $at, $t9

lb $t4, 0x0007($t0)
; absolute value
sra $t5,$t4,0x1F
xor $t4,$t4,$t5
sub $t4,$t4,$t5



andi $t5, $at, 0080

lui $t3, FFF3
ori $t3, $t3, FF00

and $at, $at, $t3

lui $a1, 8009
;increment by number players, twice, since needs two bytes
ADDU    $a1, $a1, $a3
ADDU    $a1, $a1, $a3
lb $t2, BE70($a1)

; old
lbu $v1, BE72($a1)

; sort of controller turn, try and avoid auto-center there
addi $v1, $v1, 0x10

; if new one is bigger
slt $v1, $v1, $t4

; if new smaller
beq $v1, zero, EndNewAnalogCheck
addiu $v0, $zero, 0

addiu $v0, $zero, 1


EndNewAnalogCheck:
; Dead zone check, don't detect and auto-recenter if dead zone
sltiu $v1, $t4, 0x10

sb $t4, BE72($a1)

beql $t6, $a0, NoCUp
addi $t2, $t2, 0xFFFE

NoCUp:
beql $t7, $t9, NoCDown
addi $t2, $t2, 2

NoCDown:
slti $T8, $t2, 0xFFBB
bnel $T8, $zero, LessThanMax
addiu $t2, $zero, 0x00BB

LessThanMin:
slti $T8, $t2, 0x0046
bne $T8, $zero, LessThanMax
nop
addiu $t2, $zero, 0x0045

LessThanMax:
andi $t2, $t2, FF

; Dead zone check, don't detect and auto-recenter if dead zone
bnel $v1, zero, NoFwd
nop

beq $t5, $zero, NoBack
nop
or $at, $at, $t9

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoBack:
or $at, $at, $a0

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoFwd:

sb $t2, BE70($a1)
or $at, $at, $t2

End:

add $v1, $at, $zero

SWL     $v1, 0x0004($sp)
SWR     $v1, 0x0007($sp)

j 80070CDC
ADDU    $at, $at, $s3







Hexen
(deadzone +/- 0x14 vertical, +/- 0x1c horizontal)

A Attack to Z 8000 to 2000
Swap D-up/down to C-up/down 0800 to 0008, 0400 to 0004
C-left/C-right 0001 to 0100, 0002 to 0200
B jump to R 4000 to 0010
Z-Dup/down to A/D-up/down


80054E58 ASM LWL/LWR
Controller activator at 800CDEC4

RAM 800685F8 Table of controls
ROM 0001E190 Table of Controls

0001E20C 2000 Z-hold (with left/right) for strafing
0001E210 0004 C-Down Use item
0001E22C 4000 B Jump
0001E23C 8000 A Attack
0001E24C 0010 R Zoom out map
0001E230 2001 Z/C-Left
0001E234 2002 Z/C-right
0001E240 0001 C-left
0001E244 0002 C-right
0001E21C 0800 D-up
0001E220 0400 D-down
0001E248 0008 C-up
0001E214 2800 Z+D-up
0001E218 2400 Z+D-down


Duke Nukem 64
Need to swap
D-up/D-down 0800 to 0004, 0400 to 0008 Aim up/down
C-up 0008 to 0010 R Jump
C-down 0004 to D-down 8000 Crouch
A 8000 to 0400 A Weapon Fwd
B 4000 to D-up 0800 Weapon Backward
R 0010 to 4000 B Use

800BC828: LWL/LWR

802AD164 Controller Player 1

800A26B4 Check controls against 0008

RAM Control SchemeChoice:
802A59F0 00 Control Stick Move P1
802A5D7C 00 Control Stick Move P2
802A6108 00 Control Stick Move P3
802A6494 00 Control Stick Move P4

To default to 0 Analog Move style
8004D848 A0400280

801C93D0 Control setup, of current scheme
Old ASM reads
800A2880: 0008
800A28CC: 0004
800A290C: 0800
800A292C: 0400

New
800A2880: 8D84000C 0800
800A28CC: 8D840010 0400
800A290C: 8D840004 0004
800A292C: 8D840000 0008

Code when changing control scheme:
8004D2D8: 0008 -> 0010
8004D2DC: 0004 -> 8000
8004D2E4: 0400 -> 0008
8004D2C0: 0800 -> 0004
8004D300: 4000 -> 0800
8004D304: 8000 -> 0400
8004D310: 0010 -> 4000

ROM: 
00002EDA: 0010
00002EDE: 8000 (and change to ORI)
00002EE6: 0008
00002EC2: 0004
00002F02: 0800
00002F06: 0400
00002F12: 4000












; # Players is 802AD160 at T9, ((Value - 802AD160) /8) = player #


800A5CE4 if 0x10 (mapped to aim down slot)

802A5990 holds current Y rotation, FFFFFFE4 min, 00000064, E4 Max, 0220($S0), S0 = 802A5770
Written to by (80099B98 main player loop),goes to 800A34EC, 800A3510, hardcoded, player 1, 0x38C per player, so 802A5AFC is player 2


Spots ROM: 000BA340 RAM: 80104740

Unsafe T9, T8, A0

@ROM:00072428

800BC828:
LUI $t0, 8010
ori $t0, $t0, 0x48D0
jr $t0
nop



@ROM:000BA4D0

801048D0:

LWL     $at, 0x0004($t9)
LWR     $at, 0x0007($t9)

lui $a1, 0008
lui $a2, 0004

and $t6, $at, $a1
and $t7, $at, $a2

lb $t4, 0x0007($t9)

; absolute value
sra $t5,$t4,0x1F
xor $t4,$t4,$t5
sub $t4,$t4,$t5


;Find Player # (* 8)
lui $t5, 802A
ori $t5, $t5, D160
sub $t2, $t9, $t5

lui $t1, 8010
ADDU    $t1, $t1, $t2

; old
lbu $v1, 4744($t1)

; sort of controller turn, try and avoid auto-center there
addi $v1, $v1, 0x10

; if new one is bigger
slt $v1, $v1, $t4

; if new smaller
beq $v1, zero, EndNewAnalogCheck
addiu $v0, $zero, 0

addiu $v0, $zero, 1


EndNewAnalogCheck:
; Dead zone check, don't detect and auto-recenter if dead zone
sltiu $v1, $t4, 0x10

sb $t4, 4744($t1)

; Dead zone check, don't detect and auto-recenter if dead zone
bnel $v1, zero, End
nop

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, End
nop
beq $t6, $a1, End
nop
beq $t7, $a2, End
nop

;Find players spot for final write of reset, if need be for auto-center
LUI     $t0, 0x802A
srl $t6, $t2, 3

SLL     $t7, $t6, 0x03
SUBU    $t7, $t7, $t6
SLL     $t7, $t7, 0x03
ADDU    $t7, $t7, $t6
SLL     $t7, $t7, 0x02
SUBU    $t7, $t7, $t6
SLL     $t7, $t7, 0x02
ADDIU   $t0, $t0, 0x5770
ADDU    $t0, $t7, $t0

addiu $t2, $zero, 0x64
sw $t2, 0220($t0)

End:
LWL     $t1, 0x0004($t9)
LWR     $t1, 0x0007($t9)
SW      $t1, 0x0004($t8)
LBU     $t2, 0x0006($sp)

j 800BC838
nop
Daikatana

8020CBC4 Controller Activator P1

8018EEB4 LWL V1
8018EEB8 LWR V1

What menu in (not just 00/01)
8020A9CB

Maybe use this, 801B0E55 0009 in menu
0000 not


Controller P1 Choice
8020A9E6 0001 Reverse Fixed and 0003 Normal Fixed

Write to it
RAM: 801AFEC4
ROM: 00028EC4
A022A9E6 to change 01 V0
A020A9E6 is R0

NOT WORKING FROM ROM, can't find space

T0 Unsafe, A3 Unsafe
A3 # Players


FREE SPACE RAM:
801B28F8-801B2940 = 0x48



Spot for temp data
ROM: 0002B4D4
RAM: 801B24D4



ROM: 00007EAC


8018EEB4:
LUI $t2, 801B
ori $t2, $t2, 1C40
jr $t2
nop








ROM: 0002AC40



801B1C40:
LWL     $at, 0x0004($t0)
LWR     $at, 0x0007($t0)

; If menu, value is 1, so skip special logic 800b3c40
lui $a0, 801B
lb $a0, 0x0E55($a0)

bne $a0, $zero, End
nop

lui $a0, 0008
lui $t9, 0004

and $t6, $at, $a0
and $t7, $at, $t9

lb $t4, 0x0007($t0)
; absolute value
sra $t5,$t4,0x1F
xor $t4,$t4,$t5
sub $t4,$t4,$t5



andi $t5, $at, 0080

lui $t3, FFF3
ori $t3, $t3, FF00

and $at, $at, $t3

lui $a1, 801B
;increment by number players, twice, since needs two bytes
ADDU    $a1, $a1, $a3
ADDU    $a1, $a1, $a3
;lb $t2, 28F8($a1)
addiu $t2, $zero, 0

; old
lbu $v1, 28F9($a1)

; sort of controller turn, try and avoid auto-center there
addi $v1, $v1, 0x10

; if new one is bigger
slt $v1, $v1, $t4

; if new smaller
beq $v1, zero, EndNewAnalogCheck
addiu $v0, $zero, 0

addiu $v0, $zero, 1


EndNewAnalogCheck:
; Dead zone check, don't detect and auto-recenter if dead zone
sltiu $v1, $t4, 0x10

sb $t4, 28F9($a1)

beql $t6, $a0, NoCUp
addi $t2, $t2, 0x45

NoCUp:
beql $t7, $t9, NoCDown
addi $t2, $t2, 0xFFBB

NoCDown:
slti $T8, $t2, 0xFFBB
bnel $T8, $zero, LessThanMax
addiu $t2, $zero, 0x00BB

LessThanMin:
slti $T8, $t2, 0x0046
bne $T8, $zero, LessThanMax
nop
addiu $t2, $zero, 0x0045

LessThanMax:
andi $t2, $t2, FF

; Dead zone check, don't detect and auto-recenter if dead zone
bnel $v1, zero, NoFwd
nop

beq $t5, $zero, NoBack
nop
or $at, $at, $t9

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoBack:
or $at, $at, $a0

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoFwd:

sb $t2, 28F8($a1)
or $at, $at, $t2

End:

add $v1, $at, $zero

LWL     $v0, 0x0000($t0)
LWR     $v0, 0x0003($t0)

j 8018EEC4
nop
















Forsaken 64


80022090 P1 Control Scheme 03 is right one
to 80022093 P4 Control Scheme

Maps to ROM 00022C90 default to 03030303

8000B9E0 LWL
8000B9E4 LWR

;V0, A2, A3, S3 Unsafe
;S3 # Player






ROM: 0000C5E0


8000B9E0:
LUI $t2, 8000
ori $t2, $t2, 09D0
jr $t2
nop
nop




ROM: 000015D0



800009D0:
LWL     $v0, 0x0000($a3)
LWR     $v0, 0x0003($a3)

; If menu, value is 1, so skip special logic 800b3c40
;lui $a0, 800b
;lb $a0, 0x3c40($a0)

;bne $a0, $zero, End
;nop

lui $a0, 0008
lui $t9, 0004

and $t6, $v0, $a0
and $t7, $v0, $t9

lb $t4, 0x0003($a3)
; absolute value
sra $t5,$t4,0x1F
xor $t4,$t4,$t5
sub $t4,$t4,$t5



andi $t5, $v0, 0080

lui $t3, FFF3
ori $t3, $t3, FF00

and $v0, $v0, $t3

; add # players
lui $a1, 0000
ADDU    $a1, $a1, $s3

addiu $t2, $zero, 0

EndNewAnalogCheck:
; Dead zone check, don't detect and auto-recenter if dead zone
sltiu $v1, $t4, 0x10

beql $t6, $a0, NoCUp
addi $t2, $t2, 45

NoCUp:
beql $t7, $t9, NoCDown
addi $t2, $t2, 0xFFBB

NoCDown:
slti $T8, $t2, 0xFFBB
bnel $T8, $zero, LessThanMax
addiu $t2, $zero, 0x00BB

LessThanMin:
slti $T8, $t2, 0x0046
bne $T8, $zero, LessThanMax
nop
addiu $t2, $zero, 0x0045

LessThanMax:
andi $t2, $t2, FF

; Dead zone check, don't detect and auto-recenter if dead zone
bnel $v1, zero, NoFwd
nop

beq $t5, $zero, NoBack
nop
or $v0, $v0, $t9

;Check if aiming move, don't recenter, but only if first move
beq $zero, $zero, NoFwd
nop

NoBack:
or $v0, $v0, $a0

NoFwd:

or $v0, $v0, $t2

End:


LH      $v1, 0x0004($a3)
SWL     $v0, 0x0000($a2)
SWR     $v0, 0x0003($a2)

j 8000B9F0
nop









Shadowgate 64
reads activator at 800C2970

8009538C:	89020000	LWL     $v0, 0x0000($t0)	#(*LOAD) 800C2970
80095390:	99020003	LWR     $v0, 0x0003($t0)	#(*LOAD) 800C2973
80095394:	89030004	LWL     $v1, 0x0004($t0)	#(*LOAD) 800C2974
80095398:	99030007	LWR     $v1, 0x0007($t0)	#(*LOAD) 800C2977


Detect if in menu
800E7991 00 Not, 01 Menu

800E7989 Menu Mod, 03 is normal, 05 is text



Need disable auto-center
80034284 LB Vertical
80034288 JAL

800EC998 Current Vertical Aim Value

800386CC Choose if new negative or positive
80038704 If Less than, adjust by -1 till reach
80038724 Check against minusing double floats
8003877C If Greater than, adjust by 1 till reach
800387A8 Check Against max using double floats


80038178 C-up Check
800381B8 C-down Check
800381F8 C-Left Check
80038238 C-Right Check


800A1BD4 Lookup table for which C-passed 01,02,04,08 to 08,04,02,01, not sure why other tables here similar

800E7AEC Temp Control Value

80038410 Table Lookup of Controls, table at 800ABE30
C-Up/Down/Left/Right, 01/02/04/08


Up:
8003841C:	3C10800C	LUI     $s0, 0x800C	#
80038420:	261042E0	ADDIU   $s0, $s0, 0x42E0	#(*CONSTANT) 800C42E0
80038424:	3C01800F	LUI     $at, 0x800F	#
80038428:	C42CD98C	LWC1    F12, 0xD98C($at)	#(*LOAD) 800ED98C
8003842C:	46006321	CVT.D.S F12, F12	#
80038430:	3C01800B	LUI     $at, 0x800B	#
80038434:	D438BEB0	LDC1    F24, 0xBEB0($at)	#
80038438:	46386300	ADD.D   F12, F12, F24	#
8003843C:	3C01800B	LUI     $at, 0x800B	#
80038440:	D436BEB8	LDC1    F22, 0xBEB8($at)	#
80038444:	462CB303	DIV.D   F12, F22, F12	#
80038448:	3C01800B	LUI     $at, 0x800B	#
8003844C:	D434BEC0	LDC1    F20, 0xBEC0($at)	#
80038450:	462CA303	DIV.D   F12, F20, F12	#
80038454:	0C025BD0	JAL     0x80096F40	#
80038458:	46206320	CVT.S.D F12, F12	#
8003845C:	3C01800F	LUI     $at, 0x800F	#
80038460:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
80038464:	46020002	MUL.S   F0, F0, F2	#
80038468:	C6020000	LWC1    F2, 0x0000($s0)	#(*LOAD) 800C42E0
8003846C:	46001081	SUB.S   F2, F2, F0	#
80038470:	E6020000	SWC1    F2, 0x0000($s0)	#(*STORE) 800C42E0
80038474:	3C01800F	LUI     $at, 0x800F	#
80038478:	C420D98C	LWC1    F0, 0xD98C($at)	#(*LOAD) 800ED98C
8003847C:	46000021	CVT.D.S F0, F0	#(*ASCII) F..!
80038480:	46380000	ADD.D   F0, F0, F24	#(*ASCII) F8..
80038484:	4620B583	DIV.D   F22, F22, F0	#
80038488:	4636A503	DIV.D   F20, F20, F22	#
8003848C:	0800E180	J       0x80038600	#
80038490:	4620A320	CVT.S.D F12, F20	#

80038600:	0C025610	JAL     0x80095840	#
80038604:	00000000	NOP	#
80038608:	3C01800F	LUI     $at, 0x800F	#
8003860C:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
80038610:	46020002	MUL.S   F0, F0, F2	#
80038614:	C6020008	LWC1    F2, 0x0008($s0)	#(*LOAD) 800C42E8
80038618:	46001081	SUB.S   F2, F2, F0	#
8003861C:	0800E197	J       0x8003865C	#
80038620:	E6020008	SWC1    F2, 0x0008($s0)	#

Down:
80038494:	3C10800C	LUI     $s0, 0x800C	#
80038498:	261042E0	ADDIU   $s0, $s0, 0x42E0	#(*CONSTANT) 800C42E0
8003849C:	3C01800F	LUI     $at, 0x800F	#
800384A0:	C42CD98C	LWC1    F12, 0xD98C($at)	#(*LOAD) 800ED98C
800384A4:	46006321	CVT.D.S F12, F12	#
800384A8:	3C01800B	LUI     $at, 0x800B	#
800384AC:	D438BEC8	LDC1    F24, 0xBEC8($at)	#
800384B0:	46386300	ADD.D   F12, F12, F24	#
800384B4:	3C01800B	LUI     $at, 0x800B	#
800384B8:	D436BED0	LDC1    F22, 0xBED0($at)	#
800384BC:	462CB303	DIV.D   F12, F22, F12	#
800384C0:	3C01800B	LUI     $at, 0x800B	#
800384C4:	D434BED8	LDC1    F20, 0xBED8($at)	#
800384C8:	462CA303	DIV.D   F12, F20, F12	#
800384CC:	0C025BD0	JAL     0x80096F40	#
800384D0:	46206320	CVT.S.D F12, F12	#
800384D4:	3C01800F	LUI     $at, 0x800F	#
800384D8:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
800384DC:	46020002	MUL.S   F0, F0, F2	#
800384E0:	C6020000	LWC1    F2, 0x0000($s0)	#(*LOAD) 800C42E0
800384E4:	46020000	ADD.S   F0, F0, F2	#
800384E8:	E6000000	SWC1    F0, 0x0000($s0)	#(*STORE) 800C42E0
800384EC:	3C01800F	LUI     $at, 0x800F	#
800384F0:	C420D98C	LWC1    F0, 0xD98C($at)	#(*LOAD) 800ED98C
800384F4:	46000021	CVT.D.S F0, F0	#(*ASCII) F..!
800384F8:	46380000	ADD.D   F0, F0, F24	#(*ASCII) F8..
800384FC:	4620B583	DIV.D   F22, F22, F0	#
80038500:	4636A503	DIV.D   F20, F20, F22	#
80038504:	0800E15D	J       0x80038574	#
80038508:	4620A320	CVT.S.D F12, F20	#

80038574:	0C025610	JAL     0x80095840	#
80038578:	00000000	NOP	#
8003857C:	3C01800F	LUI     $at, 0x800F	#
80038580:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
80038584:	46020002	MUL.S   F0, F0, F2	#
80038588:	C6020008	LWC1    F2, 0x0008($s0)	#(*LOAD) 800C42E8
8003858C:	46020000	ADD.S   F0, F0, F2	#
80038590:	0800E197	J       0x8003865C	#
80038594:	E6000008	SWC1    F0, 0x0008($s0)	#

Left:
8003850C:	3C10800C	LUI     $s0, 0x800C	#
80038510:	261042E0	ADDIU   $s0, $s0, 0x42E0	#(*CONSTANT) 800C42E0
80038514:	3C0143B4	LUI     $at, 0x43B4	#
80038518:	4481A000	MTC1    $at, F20	#
8003851C:	00000000	NOP	#
80038520:	3C01800F	LUI     $at, 0x800F	#
80038524:	C42CD98C	LWC1    F12, 0xD98C($at)	#(*LOAD) 800ED98C
80038528:	460CA303	DIV.S   F12, F20, F12	#
8003852C:	46006321	CVT.D.S F12, F12	#
80038530:	3C01800B	LUI     $at, 0x800B	#
80038534:	D436BEE0	LDC1    F22, 0xBEE0($at)	#
80038538:	462CB303	DIV.D   F12, F22, F12	#
8003853C:	0C025BD0	JAL     0x80096F40	#
80038540:	46206320	CVT.S.D F12, F12	#
80038544:	3C01800F	LUI     $at, 0x800F	#
80038548:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
8003854C:	46020002	MUL.S   F0, F0, F2	#
80038550:	C6020000	LWC1    F2, 0x0000($s0)	#(*LOAD) 800C42E0
80038554:	46020000	ADD.S   F0, F0, F2	#
80038558:	E6000000	SWC1    F0, 0x0000($s0)	#(*STORE) 800C42E0
8003855C:	3C01800F	LUI     $at, 0x800F	#
80038560:	C420D98C	LWC1    F0, 0xD98C($at)	#(*LOAD) 800ED98C
80038564:	4600A503	DIV.S   F20, F20, F0	#
80038568:	4600A521	CVT.D.S F20, F20	#
8003856C:	4634B583	DIV.D   F22, F22, F20	#
80038570:	4620B320	CVT.S.D F12, F22	#
80038574:	.XREF:80038504[U],
80038574:	0C025610	JAL     0x80095840	#
80038578:	00000000	NOP	#
8003857C:	3C01800F	LUI     $at, 0x800F	#
80038580:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
80038584:	46020002	MUL.S   F0, F0, F2	#
80038588:	C6020008	LWC1    F2, 0x0008($s0)	#(*LOAD) 800C42E8
8003858C:	46020000	ADD.S   F0, F0, F2	#
80038590:	0800E197	J       0x8003865C	#
80038594:	E6000008	SWC1    F0, 0x0008($s0)	#

Right:
80038598:	3C10800C	LUI     $s0, 0x800C	#
8003859C:	261042E0	ADDIU   $s0, $s0, 0x42E0	#(*CONSTANT) 800C42E0
800385A0:	3C0143B4	LUI     $at, 0x43B4	#
800385A4:	4481A000	MTC1    $at, F20	#
800385A8:	00000000	NOP	#
800385AC:	3C01800F	LUI     $at, 0x800F	#
800385B0:	C42CD98C	LWC1    F12, 0xD98C($at)	#(*LOAD) 800ED98C
800385B4:	460CA303	DIV.S   F12, F20, F12	#
800385B8:	46006321	CVT.D.S F12, F12	#
800385BC:	3C01800B	LUI     $at, 0x800B	#
800385C0:	D436BEE8	LDC1    F22, 0xBEE8($at)	#
800385C4:	462CB303	DIV.D   F12, F22, F12	#
800385C8:	0C025BD0	JAL     0x80096F40	#
800385CC:	46206320	CVT.S.D F12, F12	#
800385D0:	3C01800F	LUI     $at, 0x800F	#
800385D4:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
800385D8:	46020002	MUL.S   F0, F0, F2	#
800385DC:	C6020000	LWC1    F2, 0x0000($s0)	#(*LOAD) 800C42E0
800385E0:	46001081	SUB.S   F2, F2, F0	#
800385E4:	E6020000	SWC1    F2, 0x0000($s0)	#(*STORE) 800C42E0
800385E8:	3C01800F	LUI     $at, 0x800F	#
800385EC:	C420D98C	LWC1    F0, 0xD98C($at)	#(*LOAD) 800ED98C
800385F0:	4600A503	DIV.S   F20, F20, F0	#
800385F4:	4600A521	CVT.D.S F20, F20	#
800385F8:	4634B583	DIV.D   F22, F22, F20	#
800385FC:	4620B320	CVT.S.D F12, F22	#
80038600:	.XREF:8003848C[U],
80038600:	0C025610	JAL     0x80095840	#
80038604:	00000000	NOP	#
80038608:	3C01800F	LUI     $at, 0x800F	#
8003860C:	C422D914	LWC1    F2, 0xD914($at)	#(*LOAD) 800ED914
80038610:	46020002	MUL.S   F0, F0, F2	#
80038614:	C6020008	LWC1    F2, 0x0008($s0)	#(*LOAD) 800C42E8
80038618:	46001081	SUB.S   F2, F2, F0	#
8003861C:	0800E197	J       0x8003865C	#
80038620:	E6020008	SWC1    F2, 0x0008($s0)	#







ROM:00013C10
RAM:80038810 Store to spot again for float from menus (B press)

ROM:00013C48
RAM:80038848 Store to spot again for float from menus (B press)



800387E0: Store 0 to spot



; Only allow control move when is set 01 mode from controller to avoid twitches
ROM: 00013ABC

800386BC:
j 800AAAD0
nop



ROM 00085ED0

800AAAD0:
LBU     $v1, 0x7AF0($v1)
ANDI    $v0, $v1, 0x0003

lui $t0, 800B
lb $t0, B4C0($t0)

beq $t0, $zero, Skip
nop

j 800386C4
nop

Skip:
j 800388C8
nop

; F2 current
; F0 new

ROM: 00013AE8

;Fix so can go backwards, not just one direction till flip
800386E8:
j 800AAA10
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop


;ROM 00085E10


800AAA10:
C.LT.S  F0, F2
NOP
BC1F    NextCheckPos
NOP
LUI     $at, 0x3F80
MTC1    $at, F0
NOP
SUB.S   F0, F2, F0
SWC1    F0, 0x0000($v0)
LUI     $v0, 0x800F
ADDIU   $v0, $v0, 0xC998
NextCheckPos:
C.LT.S  F2, F0
NOP
BC1F    GoBackPositive
NOP
LUI     $at, 0x3F80
MTC1    $at, F0
NOP
ADD.S   F0, F2, F0
SWC1    F0, 0x0000($v0)
LUI     $v0, 0x800F
ADDIU   $v0, $v0, 0xC998
GoBackPositive:
j 80038714
nop

;ROM 00013B6C

8003876C:
j 800AAA70
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop


ROM 00085E70


800AAA70:

C.LT.S  F2, F0
NOP
BC1F    NextCheckNeg
NOP
LUI     $at, 0x3F80
MTC1    $at, F0
NOP
ADD.S   F0, F2, F0
SWC1    F0, 0x0000($v0)
LUI     $v0, 0x800F
ADDIU   $v0, $v0, 0xC998
NextCheckNeg:
C.LT.S  F0, F2
NOP
BC1F    GoBackNegative
NOP
LUI     $at, 0x3F80
MTC1    $at, F0
NOP
SUB.S   F0, F2, F0
SWC1    F0, 0x0000($v0)
LUI     $v0, 0x800F
ADDIU   $v0, $v0, 0xC998
GoBackNegative:
j 80038798
nop




Unsafe A3 = # players, T0

ROM 000868A8 Text for Data
RAM 800AB4A8

ROM: 0007078C

8009538C:
LUI $t2, 800A
ori $t2, $t2, A890
jr $t2
nop



ROM 00085C90


800AA890:

LWL     $at, 0x0004($t0)
LWR     $at, 0x0007($t0)

lui $a1, 800B

;Skip unconnected controllers
addi $t9, $zero, 0xFFFF
beq $at, $t9, NoWrite
nop

; If menu, value is 1, so skip special logic 800b3c40
lui $a0, 800E
lb $a0, 0x7989($a0)
addiu $t9, $zero, 3

bne $a0, $t9, End
nop


lui $a0, 0008
lui $t9, 0004

and $t6, $at, $a0
and $t7, $at, $t9

lb $t4, 0x0007($t0)
; absolute value
sra $t5,$t4,0x1F
xor $t4,$t4,$t5
sub $t4,$t4,$t5



andi $t5, $at, 0080

lui $t3, FFF3
ori $t3, $t3, FF00

and $at, $at, $t3


;increment by number players, twice, since needs two bytes
ADDU    $a1, $a1, $a3
ADDU    $a1, $a1, $a3
lb $t2, B4A8($a1)

; old
lbu $v1, B4A9($a1)

; sort of controller turn, try and avoid auto-center there
addi $v1, $v1, 0x10

; if new one is bigger
slt $v1, $v1, $t4

; if new smaller
beq $v1, zero, EndNewAnalogCheck
addiu $v0, $zero, 0

addiu $v0, $zero, 1


EndNewAnalogCheck:
; Dead zone check, don't detect and auto-recenter if dead zone
sltiu $v1, $t4, 0x10

sb $t4, B4A9($a1)

beql $t6, $a0, NoCUp
addi $t2, $t2, 1

NoCUp:
beql $t7, $t9, NoCDown
addi $t2, $t2, 0xFFFF

NoCDown:
slti $T8, $t2, 0xFFBB
bnel $T8, $zero, LessThanMax
addiu $t2, $zero, 0x00BB

LessThanMin:
slti $T8, $t2, 0x0046
bne $T8, $zero, LessThanMax
nop
addiu $t2, $zero, 0x0045

LessThanMax:
; Dead zone check, don't detect and auto-recenter if dead zone
bnel $v1, zero, NoFwd
nop

beq $t5, $zero, NoBack
nop
or $at, $at, $t9

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoBack:
or $at, $at, $a0

;Check if aiming move, don't recenter, but only if first move
beq $v0, $zero, NoFwd
nop
beq $t6, $a0, NoFwd
nop
beql $t7, $t9, NoFwd
nop

beq $zero, $zero, NoFwd
addiu $t2, $zero, 0

NoFwd:

sb $t2, B4A8($a1)

slt $t3, $zero, $t2
bne $t3, $zero, Positive
nop

slt $t3, $t2, $zero
bne $t3, $zero, Negative
nop

beq $zero, $zero, AndValue
nop

Positive:
addi $t2, $t2, 0x0010

beq $zero, $zero, AndValue
nop

Negative:
addi $t2, $t2, 0xFFF0

beq $zero, $zero, AndValue
nop

AndValue:
andi $t2, $t2, FF

or $at, $at, $t2

addiu $t7, $zero, 1

beq $zero, $zero, NoWrite
sb $t7, B4C0($a1)

End:
sb $zero, B4C0($a1)


NoWrite:

add $v1, $at, $zero

LWL     $v0, 0x0000($t0)
LWR     $v0, 0x0003($t0)

j 8009539C
nop



Mortal Kombat Mythlogies (U)
[Spots but didn't end up using, had to move at end using ASM to not set the menus, etc inadvertantly since shared]
ROM 000151DC ASM 800145DC: 801114C8 B: 0001 Turn -> 0010 High Punch
ROM 000151F8 ASM 800145F8: 801114CA R: 0004 Run -> 0001 Turn
ROM 00015214 ASM 80014614: 801114CC A: 0002 Action -> 0080 Low Punch
ROM 00015230 ASM 80014630: 801114CE L: 0008 Block -> 0002 Action
ROM 00015284 ASM 80014684: 801114D4 C-Down: 0080 Low Punch -> 0004 Run
ROM 00015268 ASM 80014668: 801114D6 C-Right: 0040 Low Kick -> 0040 Low Kick
ROM 0001524C ASM 8001464C: 801114D8 C-Up: 0020 High Kick -> 0020 High Kick
ROM 000152A0 ASM 800146A0: 801114DA C-Left: 0010 High Punch -> 0008 Block

Set manually since globally messed it up for menus and things
00100001 00800002 X00200040 00040008
X01000200 X04000800 

ROM: 000153A4

800147A4:
J 800A9B00
nop

0802A6C0
00000000

ROM: 000AA700

800A9B00:
lui $t1, 8011
ori $t1, $t1, 14C8

lui $t0, 0010
ori $t0, $t0, 0001
sw $t0, 0($t1)

lui $t0, 0080
ori $t0, $t0, 0002
sw $t0, 4($t1)

lui $t0, 0004
ori $t0, $t0, 0008
sw $t0, C($t1)

j 800147AC
nop



3C098011
352914C8
3C080010
35080001
AD280000
3C080080
35080002
AD280004
3C080004
35080008
AD28000C
080051EB
00000000


Change from JAL 8002B05C to JAL 80033CC8 to flip during normal punch/kick
ROM:0002AA18
ROM:0002ACD0
ROM:0002B04C
ROM:0002B0B8
ROM:0002A820



Kirby 64 US
8002FDE4
j 80040D10
nop

80040D10
;t0,t1,t2,t3,t4,t5,t6,t9 is free
LWL     $t8, 0x0004($v0)
LWR     $t8, 0x0007($v0)

;t0 LR Analog
LB $t0, 0x0006($v0)

;t1 UD Analog
LB $t1, 0x0007($v0)

;t3 deadzone LR
addiu $t6, $t0, 0
sra $t3,$t6,1F
xor $t6,$t6,$t3
sub $t3,$t6,$t3
sltiu $t3, $t3, 0x10

bne $t3, $zero, skipLR
nop

lui $t9, FCFF
ori $t9, $t9, FFFF
and $t8, $t8, $t9

;if less than zero
slt $t3, $t0, $zero

beq $t3, $zero, negativeLR
nop

positiveLR:
lui $t9, 0200
or $t8, $t8, $t9

beq $zero, $zero, skipLR
nop

negativeLR:
lui $t9, 0100
or $t8, $t8, $t9

beq $zero, $zero, skipLR
nop

skipLR:

;t4 deadzone UD
addiu $t6, $t1, 0
sra $t4,$t6,1F
xor $t6,$t6,$t4
sub $t4,$t6,$t4
sltiu $t4, $t4, 0x10

bne $t4, $zero, skipUD
nop

lui $t9, F3FF
ori $t9, $t9, FFFF
and $t8, $t8, $t9

;if less than zero
slt $t4, $t1, $zero

beq $t4, $zero, negativeUD
nop

positiveUD:
lui $t9, 0400
or $t8, $t8, $t9

beq $zero, $zero, skipUD
nop

negativeUD:
lui $t9, 0800
or $t8, $t8, $t9

skipUD:
j 8002FDEC
nop




08010344
00000000

88580004
98580007
80480006
80490007
250E0000
000E5FC3
01CB7026
01CB5822
2D6B0010
140B000F
00000000
3C19FCFF
3739FFFF
0319C024
0100582A
100B0005
00000000
3C190200
0319C025
10000005
00000000
3C190100
0319C025
10000001
00000000
252E0000
000E67C3
01CC7026
01CC6022
2D8C0010
140C000D
00000000
3C19F3FF
3739FFFF
0319C024
0120602A
100C0005
00000000
3C190400
0319C025
10000003
00000000
3C190800
0319C025
0800BF7B
00000000

Yoshi's Story US
Read for resize fruit (branch always to skip)
Change to CButton 0008 and 0004
80321BB0:	24010800	ADDIU   $at, $zero, 0x0800	#
80321BC0:	30590800	ANDI    $t9, $v0, 0x0800	#
80321BC4:	1721004B	BNE     $t9, $at, 0x80321CF4	#ROM 00CB17E4


80321CF4:	30590400	ANDI    $t9, $v0, 0x0400	#
80321CF8:	24010400	ADDIU   $at, $zero, 0x0400	#
80321CFC:	17210051	BNE     $t9, $at, 0x80321E44	#ROM 00CB191C




;ROM 00065550
80064950
j 800B1820
nop
nop
nop
;Main Section - WORKING
;ROM 000B2420
800B1820

LWL     $t7, 0x0000($v1)
LWL     $t6, 0x0004($v1)
LWR     $t7, 0x0003($v1)
LWR     $t6, 0x0007($v1)

start:


lui $t5, 800B
addiu $t4, $zero, FFFF

;D-Right
lui $t0, 0100
and $t0, $t7, $t0

beql $t0, $zero, skipDRight
nop

lui $t0, FEFF
ori $t0, $t0, 00FF

and $t7, $t0, $t7

ori $t7, $t7, 7F00
beq $zero, $zero, skipDLeft
nop

skipDRight:

;D-Left
lui $t1, 0200
and $t1, $t7, $t1

beq $t1, $zero, skipDLeft
nop

lui $t1, FDFF
ori $t1, $t1, 00FF

and $t7, $t1, $t7

ori $t7, $t7, 8000

skipDLeft:


;D-Down
lui $t2, 0400
and $t2, $t7, $t2

beq $t2, $zero, skipDUp
nop

lui $t2, FBFF
ori $t2, $t2, FF00

and $t7, $t2, $t7

ori $t7, $t7, 0080
beq $zero, $zero, goback
nop


skipDUp:
;D-Up
lui $t3, 0800
and $t3, $t7, $t3

beq $t3, $zero, goback
nop

lui $t3, F7FF
ori $t3, $t3, FF00

and $t7, $t3, $t7

ori $t7, $t7, 007F
beq $zero, $zero, goback
nop

goback:
j 80064960
nop









Mischief Makers

;ROM 0009B640
8009AA40
j 800EB730
addiu $t5, $zero, 0

;ROM 0009BFB8
8009B3B8
j 800EB730
addiu $t5, $zero, 1


;Main Section
;ROM 000EC330
800EB730

start:
LWL     $t1, 0x0004($t9)
LWR     $t1, 0x0007($t9)

;t0 LR Analog
LB $t0, 0x0006($t9)

;t8 UD Analog
LB $t4, 0x0007($t9)

;t3 deadzone LR
addiu $t6, $t0, 0
sra $t3,$t6,1F
xor $t6,$t6,$t3
sub $t3,$t6,$t3
sltiu $t3, $t3, 0x10

bne $t3, $zero, skipLR
nop

lui $t7, FCFF
ori $t7, $t7, FFFF
and $t1, $t1, $t7

;if less than zero
slt $t3, $t0, $zero

beq $t3, $zero, negativeLR
nop

positiveLR:
lui $t7, 0200
or $t1, $t1, $t7

beq $zero, $zero, skipLR
nop

negativeLR:
lui $t7, 0100
or $t1, $t1, $t7

beq $zero, $zero, skipLR
nop

skipLR:

;t3 deadzone UD
addiu $t6, $t4, 0
sra $t3,$t6,1F
xor $t6,$t6,$t3
sub $t3,$t6,$t3
sltiu $t3, $t3, 0x10

bne $t3, $zero, skipUD
nop

lui $t7, F3FF
ori $t7, $t7, FFFF
and $t1, $t1, $t7

;if less than zero
slt $t3, $t4, $zero

beq $t3, $zero, negativeUD
nop

positiveUD:
lui $t7, 0400
or $t1, $t1, $t7

beq $zero, $zero, skipUD
nop

negativeUD:
lui $t7, 0800
or $t1, $t1, $t7

skipUD:


bne $t5, $zero, back2
nop

j 8009AA48
nop

back2:
j 8009B3C0
nop



Bomberman 64 Arcade J

;ROM 000351D4
802341D4
j 80242490
addiu $at, $zero, 0

;This seemed to mess stuff up
;ROM 000354D4
;802344D4
;j 80242490
;addiu $at, $zero, 1


;REPLACE MPAL Video
;Main Section
;ROM 00043490
80242490

start:
LWL     $v1, 0x0004($t0)
LWR     $v1, 0x0007($t0)

;t8 LR Analog
LB $t8, 0x0006($t0)

;t4 UD Analog
LB $t9, 0x0007($t0)

;t3 deadzone LR
addiu $t6, $t8, 0
sra $t3,$t6,1F
xor $t6,$t6,$t3
sub $t3,$t6,$t3
sltiu $t3, $t3, 0x10

bne $t3, $zero, skipLR
nop

lui $t7, FCFF
ori $t7, $t7, FFFF
and $v1, $v1, $t7

;if less than zero
slt $t3, $t8, $zero

beq $t3, $zero, negativeLR
nop

positiveLR:
lui $t7, 0200
or $v1, $v1, $t7

beq $zero, $zero, skipLR
nop

negativeLR:
lui $t7, 0100
or $v1, $v1, $t7

beq $zero, $zero, skipLR
nop

skipLR:

;t3 deadzone UD
addiu $t6, $t9, 0
sra $t3,$t6,1F
xor $t6,$t6,$t3
sub $t3,$t6,$t3
sltiu $t3, $t3, 0x10

bne $t3, $zero, skipUD
nop

lui $t7, F3FF
ori $t7, $t7, FFFF
and $v1, $v1, $t7

;if less than zero
slt $t3, $t9, $zero

beq $t3, $zero, negativeUD
nop

positiveUD:
lui $t7, 0400
or $v1, $v1, $t7

beq $zero, $zero, skipUD
nop

negativeUD:
lui $t7, 0800
or $v1, $v1, $t7

skipUD:


bne $at, $zero, back2
nop

j 802341DC
nop

back2:
j 802344DC
nop



Super Smash Brothers US
;ROM 00032C24
80032024
j 8003D910
nop






;ROM 0003E510 - Gradual up/down left/right
8003D910
start:
;8003DDA0 Store variable for LR ROM 0003E990
;8003DDA1 Store variable for UD ROM 0003E991

LWL     $t8, 0x0004($v0)
LWR     $t8, 0x0007($v0)


;C-Up Check
lui $t0, 0008
and $t0, $t8, $t0

;L-Check
lui $t1, 0020
and $t1, $t8, $t1

lui $t6, FFD7
ori $t6, $t6, FFFF
and $t8, $t6, $t8

beql $t0, $zero, skipCUp
nop

lui $t0, 0020
or $t8, $t0, $t8

skipCUp:

beql $t1, $zero, skipL
nop

lui $t1, 2000
or $t8, $t1, $t8

skipL:


lui $t5, 8004
addu $t5, $t5, $v1
addu $t5, $t5, $v1
addiu $t4, $zero, FFFF

lui $t0, 0300
and $t0, $t8, $t0
beql $t0, $zero, skipDLeft
sb $zero, DDA0($t5)

;D-Right
lui $t0, 0100
and $t0, $t8, $t0

beql $t0, $zero, skipDRight
nop

lui $t0, FEFF
ori $t0, $t0, 00FF

and $t8, $t0, $t8

lbu $t6, DDA0($t5)

checkR20:
addiu $at, $zero, 2A
bne $t6, $at, checkR32
nop
addiu $at, $zero, 54
sb $at, DDA0($t5)
beq $zero, $zero, skipDLeft
ori $t8, $t8, 5400

checkR32:
addiu $at, $zero, 54
beq $t6, $at, setR45
nop

addiu $at, $zero, 7F
bne $t6, $at, defaultR
nop

setR45:
addiu $at, $zero, 7F
sb $at, DDA0($t5)
beq $zero, $zero, skipDLeft
ori $t8, $t8, 7F00


defaultR:
addiu $at, $zero, 2A
sb $at, DDA0($t5)
ori $t8, $t8, 2A00
beq $zero, $zero, skipDLeft
nop

skipDRight:

;D-Left
lui $t1, 0200
and $t1, $t8, $t1

beq $t1, $zero, skipDLeft
nop

lui $t1, FDFF
ori $t1, $t1, 00FF

and $t8, $t1, $t8

lbu $t6, DDA0($t5)

checkLE0:
addiu $at, $zero, D6
bne $t6, $at, checkLCE
nop
addiu $at, $zero, AC
sb $at, DDA0($t5)
beq $zero, $zero, skipDLeft
ori $t8, $t8, AC00

checkLCE:
addiu $at, $zero, AC
beq $t6, $at, setLBB
nop

addiu $at, $zero, 80
bne $t6, $at, defaultL
nop

setLBB:
addiu $at, $zero, 80
sb $at, DDA0($t5)
beq $zero, $zero, skipDLeft
ori $t8, $t8, 8000


defaultL:
addiu $at, $zero, D6
sb $at, DDA0($t5)
ori $t8, $t8, D600

skipDLeft:

lui $t0, 0C00
and $t0, $t8, $t0
beql $t0, $zero, goback
sb $zero, DDA1($t5)

;D-Down
lui $t2, 0400
and $t2, $t8, $t2

beq $t2, $zero, skipDUp
nop

lui $t2, FBFF
ori $t2, $t2, FF00

and $t8, $t2, $t8

lbu $t6, DDA1($t5)

checkDE0:
addiu $at, $zero, D6
bne $t6, $at, checkDCE
nop
addiu $at, $zero, AC
sb $at, DDA1($t5)
beq $zero, $zero, goback
ori $t8, $t8, 00AC

checkDCE:
addiu $at, $zero, AC
beq $t6, $at, setDBB
nop

addiu $at, $zero, 80
bne $t6, $at, defaultD
nop

setDBB:
addiu $at, $zero, 80
sb $at, DDA1($t5)
beq $zero, $zero, goback
ori $t8, $t8, 0080


defaultD:
addiu $at, $zero, D6
sb $at, DDA1($t5)
ori $t8, $t8, 00D6
beq $zero, $zero, goback
nop


skipDUp:
;D-Up
lui $t3, 0800
and $t3, $t8, $t3

beq $t3, $zero, goback
nop

lui $t3, F7FF
ori $t3, $t3, FF00

and $t8, $t3, $t8

lbu $t6, DDA1($t5)

checkU20:
addiu $at, $zero, 2A
bne $t6, $at, checkU32
nop
addiu $at, $zero, 54
sb $at, DDA1($t5)
beq $zero, $zero, goback
ori $t8, $t8, 0054

checkU32:
addiu $at, $zero, 54
beq $t6, $at, setU45
nop

addiu $at, $zero, 7F
bne $t6, $at, defaultU
nop

setU45:
addiu $at, $zero, 7F
sb $at, DDA1($t5)
beq $zero, $zero, goback
ori $t8, $t8, 007F


defaultU:
addiu $at, $zero, 2A
sb $at, DDA1($t5)
ori $t8, $t8, 002A
beq $zero, $zero, goback
nop

goback:
j 8003202C
nop



Jet Force Gemini US

;t0, t2, t3, t4, t5, t6, t7 free

; ROM 00098A08
80097E08
j 800AA22C
nop

; ROM 000AAE2C
800AA22C
LWL     $t1, 0x0004($t9)
LWR     $t1, 0x0007($t9)
lbu $t2, 0x0005($t9)
lb $t3, 0x0007($t9)

;800A4FC4 Mode 00 Solo 01 Multi
;800A506C Mode of Multi 02 Target

;800F804C Read Pointer for +10D in solo
; 00 No Floyd, 01 Floyd, 02 Scene

; Solo or Multi Mode Check
lui $t0, 800A
lb $t0, 4FC4($t0)

beq $t0, $zero, solocheck
nop

lui $t0, 800A
lb $t0, 506C($t0)

; If Target Mode
addiu $t6, $zero, 2
beq $t0, $t6, doinvert
nop

addiu $t6, $zero, 0
beq $t0, $t6, RCheck
nop

beq $zero, $zero, back
nop

solocheck:
; Floyd Check
lui $t0, 8010
lw $t0, 804C($t0)

beq $t0, $zero, back
addiu $t6, $zero, 1

lb $t0, 10D($t0)

beq $t0, $t6, doinvert
nop

bne $t0, zero, back
nop

RCheck:
andi $t4, $t2, 0010
beq $t4, $zero, back
nop

doinvert:
sub $t3, $zero, $t3
andi $t3, $t3, 00FF
lui $t5, FFFF
ori $t5, $t5, FF00
and $t1, $t1, $t5
or $t1, $t1, $t3


back:
j 80097E10
nop