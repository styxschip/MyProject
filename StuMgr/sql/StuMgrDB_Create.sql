USE master -- 设置当前数据库为master,以便访问sysdatabases表
GO

IF EXISTS(SELECT * FROM sysdatabases WHERE name='StuMgrDB')
DROP DATABASE StuMgrDB
GO

CREATE DATABASE StuMgrDB
GO

USE StuMgrDB
GO

CREATE TABLE [dbo].[tbl_class] (
	[cid] [int] NOT NULL ,
	[cname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tbl_teacher] (
	[tid] [int] NOT NULL ,
	[tname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tbl_course] (
	[couid] [int] NOT NULL ,
	[couname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tbl_select_course] (
	[sid] [int] NOT NULL ,
	[couid] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[score] [float] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[tbl_student] (
	[sid] [int] NOT NULL ,
	[sname] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[tbl_class] WITH NOCHECK ADD 
	 PRIMARY KEY  CLUSTERED 
	(
		[cid]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[tbl_teacher] WITH NOCHECK ADD 
	 PRIMARY KEY  CLUSTERED 
	(
		[tid]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[tbl_course] WITH NOCHECK ADD 
	 PRIMARY KEY  CLUSTERED 
	(
		[couid]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[tbl_select_course] WITH NOCHECK ADD 
	 PRIMARY KEY  CLUSTERED 
	(
		[sid],
		[couid]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[tbl_student] WITH NOCHECK ADD 
	 PRIMARY KEY  CLUSTERED 
	(
		[sid]
	)  ON [PRIMARY] 
GO
