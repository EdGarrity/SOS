USE [SOS]
GO

/****** Object:  Table [dbo].[ProgressLog]    Script Date: 1/20/2019 9:16:21 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[ProgressLog](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[RowVersion] [timestamp] NOT NULL,
	[Create_DTS] [datetime] NOT NULL,
	[Update_DTS] [datetime] NOT NULL,
	[Generation] [int] NOT NULL,
	[Group_TrainingScore] [float] NULL,
	[Group_TestScore] [float] NULL,
	[EligibleParents_TrainingScore] [float] NULL,
	[EligibleParents_TestScore] [float] NULL,
	[TestCase_Best_Individuals_TrainingScore] [float] NOT NULL,
	[TestCase_Best_leIndividuals_TestScore] [float] NOT NULL,
	[BestIndividual_TrainingScore] [float] NOT NULL,
	[BestIndividual_TestScore] [float] NOT NULL,
	[Training_Sscore_of_Eelite_Individual_with_Maximum_Number_Test_Cases] [float] NULL,
	[Test_Sscore_of_Eelite_Individual_with_Maximum_Number_Test_Cases] [float] NULL,
	[Elite_Size] [int] NOT NULL,
	[Elite_TestCases] [int] NOT NULL,
	[Total_TestCases] [int] NOT NULL,
	[Opening_Balance] [float] NOT NULL,
	[Population_Size] [int] NOT NULL,
	[Alternation_Rate] [float] NOT NULL,
	[Uniform_Mutation_Rate] [float] NOT NULL,
 CONSTRAINT [PK_ProgressLog] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[ProgressLog] ADD  CONSTRAINT [DF__ProgressLog__Creat]  DEFAULT (getdate()) FOR [Create_DTS]
GO

ALTER TABLE [dbo].[ProgressLog] ADD  CONSTRAINT [DF_ProgressLog_Update_DTS]  DEFAULT (getdate()) FOR [Update_DTS]
GO


