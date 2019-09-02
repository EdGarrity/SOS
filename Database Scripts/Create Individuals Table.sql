USE [SOS]
GO

/****** Object:  Table [dbo].[Individuals]    Script Date: 8/31/2019 10:51:33 AM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Individuals](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Created_DTS] [datetime] NOT NULL CONSTRAINT [DF_Individuals_CreatedDTS]  DEFAULT (getdate()),
	[Genome] [text] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO

