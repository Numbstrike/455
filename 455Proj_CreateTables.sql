#Jessica Baron		CSC455		Fall 2016		RPG Inventory Project: Table Creation

drop table if exists PLAYER;
drop table if exists ITEM;
drop table if exists OWNED;
drop table if exists EQUIPPED;
drop table if exists WEAPON;
drop table if exists ARMOR;
drop table if exists USABLE;

create table PLAYER
(
	name varchar(25) check not null,
	health numeric(3,0) check (health >= 0) and (health <= 100),
	stamina numeric(3,0) check (stamina >= 0) and (stamina <= 100),
	coins numeric(4,0) check (coins >= 0),
	
	primary key (name)
);


create table ITEM
(
	name varchar(25) check not null,
	baseValue numeric(4,0) check (baseValue >= 0),
	kind varchar(7) check (itemType='weapon') or (itemType='armor') or (itemType='usable') or (itemType='misc'),
	weight numeric(2,0) check (weight >= 0),

	primary key (name, kind)
);


create table OWNED
(
	playerName varchar(25) check not null,
	itemName varchar(25) check not null,
	itemType varchar(7) check (itemType='weapon') or (itemType='armor') or (itemType='usable') or (itemType='misc'),
	quantity numeric(3,0) check (quantity > 0),
	
	primary key (playerName, itemName, itemType),
	foreign key (playerName) references PLAYER(name),
	foreign key (itemName) references ITEM(name),
	foreign key (itemType) references ITEM(kind)
);#ENGINE=INNODB;


create table EQUIPPED
(
	playerName varchar(25) check not null,
	itemName varchar(25) check not null,
	itemType varchar(7) check (itemType='weapon') or (itemType='armor'),
	bodyRegion varchar(10) check not null,

	foreign key (playerName) references PLAYER(name),
	foreign key (itemName) references ITEM(name),
	foreign key (itemType) references ITEM(kind),
	unique (playerName, bodyRegion)
); #ENGINE=INNODB;


create table WEAPON
(
	name varchar(25) check not null,
	baseAttack numeric(2,0) check (baseAttack>0),
	degredation numeric(3,2) check (degredation>=0) and (degredation<=1),

	primary key (name)
) ENGINE=INNODB;


create table ARMOR
(
	name varchar(25) check not null,
	baseDefense numeric(2,0) check (baseDefense>0),
	degredation numeric(3,2) check (degredation>=0) and (degredation<=1),
	
	primary key (name)
) ENGINE=INNODB;


create table USABLE
(
	name varchar(25) check not null,
	affects varchar(7) check (affects='health') or (affects='stamina'),
	modifier numeric(3,2) check (modifier>=0),

	primary key (name)
);

