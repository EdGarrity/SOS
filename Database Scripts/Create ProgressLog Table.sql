USE [SOS]
GO

/****** Object:  Table [dbo].[ProgressLog]    Script Date: 8/31/2019 10:51:47 AM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[ProgressLog](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Created_DTS] [datetime] NOT NULL CONSTRAINT [DF_ProgressLog_Created_DTS]  DEFAULT (getdate()),
	[Generation] [int] NOT NULL,
	[Generations_Completed_This_Session] [int] NOT NULL,
	[BestIndividual_ID] [int] NOT NULL,
	[BestIndividual_Training_Score] [float] NOT NULL,
	[BestIndividual_Training_Error] [float] NOT NULL,
	[Average_Training_Error] [float] NOT NULL,
	[BestIndividual_Test_Score] [float] NOT NULL,
	[Number_Of_Training_Cases] [int] NOT NULL,
	[Number_Of_Test_Cases] [int] NOT NULL,
	[Best_Genome] [text] NOT NULL,
	[Population_Size] [int] NOT NULL,
	[Alternation_Rate] [float] NOT NULL,
	[Uniform_Mutation_Rate] [float] NOT NULL,
	[Example_Case_Max_Length] [int] NOT NULL,
	[Example_Case_Upper_Range] [int] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO

