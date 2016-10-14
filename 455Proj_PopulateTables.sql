delete from PLAYER;
delete from ITEM;
delete from OWNED;
delete from EQUIPPED;
delete from WEAPON;
delete from ARMOR;
delete from USABLE;


insert into PLAYER
values ('Aragorn', 87, 90, 580);
insert into PLAYER
values ('Shady Old Man', 50, 60, 300);
insert into PLAYER
values ('Pooky the Bear', 70, 80, 500);


insert into ITEM
values ('Steel Sword', 65, 'weapon', 15);
insert into ITEM
values ('Daedric Fork', 500, 'weapon', 4);
insert into ITEM
values ('Leather Cuirass', 70, 'armor', 35);
insert into ITEM
values ('Feather', 5, 'misc', 1);
insert into ITEM
values ('Bread', 7, 'usable', 2);
insert into ITEM
values ('Glass Dagger', 100, 'weapon',9);


insert into ITEM
('Skooma', 250, 'usable', 3);
insert into ITEM
values ('Garlic', 5, 'usable', 1);
insert into ITEM
values ('Saber Cat Steak', 50, 'usable', 10);
insert into ITEM
values ('Horse Flank', 30, 'usable', 15);
insert into ITEM
values ('Coffee Cake', 15, 'usable', 3);
insert into ITEM
values ('Dragon Gallbladder', 125, 'usable', 8);
insert into ITEM
values ('Potato', 3, 'usable', 3);
insert into ITEM
values ('Leek', 3, 'usable', 2);
insert into ITEM
values ('Canis Root', 10, 'usable', 1);
insert into ITEM
values ('Ancient Dwarven Casserole', 50, 'usable', 8);
insert into ITEM
values ('Mystery Meatloaf', 20, 'usable', 7);
insert into ITEM
values ('Cold Brew Coffee', 12, 'usable', 2);
insert into ITEM
values ('Mosquito Soup', 10, 'usable', 5);
insert into ITEM
values ('Mermaid Fin Truffles', 44, 'usable', 3);
insert into ITEM
values ('Daedric Birthday Cupcake', 95, 'usable', 5);
insert into ITEM
values ('Redoran Cheese Crackers', 20, 'usable', 4);
insert into ITEM
values ('Krill Juice', 14, 'usable', 2);
insert into ITEM
values('Random Roadside Macaroni', 22'usable', 4); 

insert into OWNED
values ('Aragorn', 'Daedric Fork', 'weapon', 2);
insert into OWNED
values ('Aragorn', 'Bread', 'usable', 1);
insert into OWNED
values ('Shady Old Man', 'Glass Dagger', 'weapon', 2);
insert into OWNED
values ('Shady Old Man', 'Steel Sword', 'weapon', 1);
insert into OWNED
values ('Pooky the Bear', 'Dragonbone Slicer', 'weapon', 1);


insert into EQUIPPED
values ('Aragorn', 'Daedric Fork', 'weapon', 'leftHand');
insert into EQUIPPED
values ('Shady Old Man', 'Glass Dagger', 'weapon', 'rightHand');
insert into EQUIPPED
values ('Pooky the Bear', 'Dragonbone Slicer', 'weapon', 'rightHand');

insert into WEAPON
values ('Steel Sword', 50, 0.75);
insert into WEAPON
values ('Daedric Fork', 55, 0.80);
insert into WEAPON
values ('Glass Dagger', 30, 0.6);
insert into WEAPON
values ('Iron Mace', 20, 0.5);
insert into WEAPON
values ('Dragonbone Slicer', 80, 0.6);
insert into WEAPON
values ('Cursed Battleaxe', 80, 0.92);


insert into ARMOR
values ('Leather Cuirass', 90, 1.00);
insert into ARMOR
values ('Leather Helmet', 80, 1.00);
insert into ARMOR
values ('Chef"s Hat', 5, 0.80);
insert into ARMOR
values ('Ebony Boots', 30, 0.64);
insert into ARMOR
values ('Silk Gloves', 1, 1.00);
insert into ARMOR
values ('Chainmail', 60, 0.38);

insert into USABLE
values ('Bread', 'stamina', 1.20);
insert into USABLE 
values ('Human Heart', 'health', 1.8);
nsert into USABLE
VALUES ('Chocolate Cake', 'health', 1.2);
insert into USABLE
values ('Cheese Wheel', 'health', 1.3);
insert into USABLE
values ('Mammoth Cheese', 'health', 1.5);
insert into USABLE
values ('Skooma', 'stamina', 1.7);
insert into USABLE
values ('Garlic', 'health', 1.05);
insert into USABLE
values ('Saber Cat Steak', 'health', 1.4);
insert into USABLE
values ('Horse Flank', 'health', 1.3);
insert into USABLE
values ('Coffee Cake', 'stamina', 1.1);
insert into USABLE
values ('Dragon Gallbladder', 'stamina', 1.9);
insert into USABLE
values ('Potato', 'health', 1.1);
insert into USABLE
values ('Leek', 'health', 1.1);
insert into USABLE
values ('Canis Root', 'stamina', .1);
insert into USABLE
values ('Ancient Dwarven Casserole', 'health', .05);
insert into USABLE
values ('Mystery Meatloaf', 'stamina', .7);
insert into USABLE
values ('Cold Brew Coffee', 'stamina', 1.3);
insert into USABLE
values ('Mosquito Soup', 'health', .9);
insert into USABLE
values ('Mermaid Fin Truffles', 'stamina', 1.2);
insert into USABLE
values ('Daedric Birthday Cupcake', 'health', 1.6);
insert into USABLE
values ('Redoran Cheese Crackers', 'stamina', 1.5);
insert into USABLE
values ('Krill Juice', 'health', 1.2);
insert into USABLE
values('Random Roadside Macaroni', 'stamina', .3);

