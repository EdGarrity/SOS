USE [SOS]
GO

/****** Object:  Table [dbo].[Individuals]    Script Date: 9/15/2019 8:44:07 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Individuals](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Created_DTS] [datetime] NOT NULL,
	[Individual_ID] [int] NOT NULL,
	[Genome] [text] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

ALTER TABLE [dbo].[Individuals] ADD  CONSTRAINT [DF_Individuals_CreatedDTS]  DEFAULT (getdate()) FOR [Created_DTS]
GO

