USE [SOS]
GO

/****** Object:  Table [dbo].[TestingProgressReport]    Script Date: 12/8/2023 3:08:41 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[TestingProgressReport](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[Created_DTS] [datetime] NOT NULL,
	[Run_Number] [int] NULL,
	[Generation] [int] NOT NULL,
	[Generations_Completed_This_Session] [int] NOT NULL,
	[Best_Strategy_ID] [int] NOT NULL,
	[Best_Strategy_Score] [float] NOT NULL,
	[Best_Strategy_Effort] [int] NOT NULL,
	[Best_Sortino_Ratio] [float] NOT NULL,
	[Best_Previous_Strategy_Score] [float] NOT NULL,
	[Best_Strategy_Avg_Score] [float] NOT NULL,
	[Best_Standard_Deviation] [float] NOT NULL,
	[Best_Strategy_Test_Score] [float] NOT NULL,
	[Number_Of_Training_Cases] [int] NOT NULL,
	[Number_Of_Test_Cases] [int] NOT NULL,
	[Tempareture] [float] NOT NULL,
	[Stalled_Count] [int] NOT NULL,
	[Cool_Down_Count] [int] NOT NULL,
	[Include_Best_Strategy_In_Breeding_Pool] [int] NOT NULL,
	[Best_Strategy_Genome] [text] NOT NULL,
	[Population_Size] [int] NOT NULL,
	[Alternation_Rate] [float] NOT NULL,
	[Uniform_Mutation_Rate] [float] NOT NULL,
	[Max_Length] [int] NOT NULL,
	[Upper_Range] [int] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

ALTER TABLE [dbo].[TestingProgressReport] ADD  CONSTRAINT [DF_TestingProgressReport_Created_DTS]  DEFAULT (getdate()) FOR [Created_DTS]
GO

