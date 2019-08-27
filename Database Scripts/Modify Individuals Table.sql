USE [SOS]
GO

ALTER TABLE [dbo].[Individuals] DROP CONSTRAINT [DF_Individuals_CreatedDTS]
GO

/****** Object:  Table [dbo].[Individuals]    Script Date: 8/25/2019 11:50:25 AM ******/
DROP TABLE [dbo].[Individuals]
GO

/****** Object:  Table [dbo].[Individuals]    Script Date: 8/25/2019 11:50:25 AM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[Individuals](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Created_DTS] [datetime] NOT NULL,
	[Genome] [text] NOT NULL
) ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO

ALTER TABLE [dbo].[Individuals] ADD  CONSTRAINT [DF_Individuals_CreatedDTS]  DEFAULT (getdate()) FOR [Created_DTS]
GO


