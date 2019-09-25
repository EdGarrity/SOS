USE [SOS]
GO

/****** Object:  Table [dbo].[Individuals]    Script Date: 9/24/2019 9:38:44 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[Individuals](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[RowVersion] [timestamp] NOT NULL,
	[Create_DTS] [datetime] NOT NULL CONSTRAINT [DF__Individuals__Creat__24927208]  DEFAULT (getdate()),
	[Update_DTS] [datetime] NOT NULL CONSTRAINT [DF_Individuals_Update_DTS]  DEFAULT (getdate()),
	[Individual_ID] [int] NOT NULL,
	[Genome] [text] NOT NULL,
	[Parent_1] [varchar](50) NOT NULL,
	[Parent_2] [varchar](50) NOT NULL,
	[Parent_1_1] [varchar](50) NOT NULL,
	[Parent_1_2] [varchar](50) NOT NULL,
	[Parent_2_1] [varchar](50) NOT NULL,
	[Parent_2_2] [varchar](50) NOT NULL,
	[Parent_1_1_1] [varchar](50) NOT NULL,
	[Parent_1_1_2] [varchar](50) NOT NULL,
	[Parent_1_2_1] [varchar](50) NOT NULL,
	[Parent_1_2_2] [varchar](50) NOT NULL,
	[Parent_2_1_1] [varchar](50) NOT NULL,
	[Parent_2_1_2] [varchar](50) NOT NULL,
	[Parent_2_2_1] [varchar](50) NOT NULL,
	[Parent_2_2_2] [varchar](50) NOT NULL,
 CONSTRAINT [PK_Individuals] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO


