USE [SOS]
GO

/****** Object:  Table [dbo].[ProgressLog]    Script Date: 11/16/2019 2:51:41 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[ProgressLog](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Created_DTS] [datetime] NOT NULL,
	[Generation] [int] NOT NULL,
	[Generations_Completed_This_Session] [int] NOT NULL,
	[BestIndividual_ID] [int] NOT NULL,
	[BestIndividual_Training_Score] [float] NOT NULL,
	[BestIndividual_Training_Error] [float] NOT NULL,
	[Average_Training_Error] [float] NOT NULL,
	[Standard_Deviation] [float] NOT NULL,
	[BestIndividual_Test_Score] [float] NOT NULL,
	[Number_Of_Training_Cases] [int] NOT NULL,
	[Number_Of_Test_Cases] [int] NOT NULL,
	[Best_Genome] [text] NOT NULL,
	[Population_Size] [int] NOT NULL,
	[Alternation_Rate] [float] NOT NULL,
	[Uniform_Mutation_Rate] [float] NOT NULL,
	[Example_Case_Max_Length] [int] NOT NULL,
	[Example_Case_Upper_Range] [int] NOT NULL,
	[Tempareture] [float] NOT NULL,
	[BestIndividual_Prev_Training_Error] [float] NOT NULL,
	[Stalled_Count] [int] NOT NULL,
	[Cool_Down_Count] [int] NOT NULL,
	[Include_Best_Individual_In_Breeding_Pool] [int] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

ALTER TABLE [dbo].[ProgressLog] ADD  CONSTRAINT [DF_ProgressLog_Created_DTS]  DEFAULT (getdate()) FOR [Created_DTS]
GO

