USE [SOS]
GO

/****** Object:  Table [dbo].[OrderMatrix]    Script Date: 8/29/2023 7:45:19 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[OrderMatrix](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Training_Case_Index] [int] NOT NULL,
	[Strategy_Index] [int] NOT NULL,
	[Order] [int] NOT NULL
) ON [PRIMARY]
GO

