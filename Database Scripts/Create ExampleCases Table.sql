USE [SOS]
GO

/****** Object:  Table [dbo].[ExampleCases]    Script Date: 8/31/2019 10:51:21 AM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[ExampleCases](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Created_DTS] [datetime] NOT NULL CONSTRAINT [DF_ExampleCases_CreatedDTS]  DEFAULT (getdate()),
	[Problem] [varchar](3000) NOT NULL,
	[Solution] [varchar](3000) NOT NULL
) ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO

