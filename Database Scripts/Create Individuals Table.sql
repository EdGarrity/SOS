USE [SOS]
GO

/****** Object:  Table [dbo].[Individuals]    Script Date: 12/23/2018 11:55:42 AM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Individuals](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[RowVersion] [timestamp] NOT NULL,
	[Create_DTS] [datetime] NOT NULL,
	[Update_DTS] [datetime] NOT NULL,
	[Genome] [varchar](5000) NOT NULL,
 CONSTRAINT [PK_Individuals] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[Individuals] ADD  CONSTRAINT [DF__Individuals__Creat__24927208]  DEFAULT (getdate()) FOR [Create_DTS]
GO

ALTER TABLE [dbo].[Individuals] ADD  CONSTRAINT [DF_Individuals_Update_DTS]  DEFAULT (getdate()) FOR [Update_DTS]
GO


